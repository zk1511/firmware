/**
 ******************************************************************************
 * @file    spi_hal.c
 * @author  Matthew McGowan
 * @version V1.0.0
 * @date    12-Sept-2014
 * @brief
 ******************************************************************************
 Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation, either
 version 3 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "spi_hal.h"

#define TOTAL_SPI 2
//#define useDMASPI

typedef enum SPI_Num_Def {
    SPI2_GROUP = 0,//user
    SPI1_GROUP = 1 //sx1278 use
} SPI_Num_Def;

typedef struct STM32_SPI_Info {
    SPI_TypeDef* SPI_Peripheral;

    DMA_Channel_TypeDef* SPI_RX_DMA_Stream;
    DMA_Channel_TypeDef* SPI_TX_DMA_Stream;

    IRQn_Type SPI_DMA_RX_IRQn;
    IRQn_Type SPI_DMA_TX_IRQn;

    GPIO_TypeDef* SPI_SCK_Port;
    GPIO_TypeDef* SPI_MISO_Port;
    GPIO_TypeDef* SPI_MOSI_Port;
    //GPIO_TypeDef SPI_SS_Port;

    uint16_t SPI_SCK_Pin;
    uint16_t SPI_MISO_Pin;
    uint16_t SPI_MOSI_Pin;
    uint16_t SPI_SS_Pin;
    uint16_t SPI_AF_Mapping;
} STM32_SPI_Info;

typedef struct SPI_State
{
    DMA_HandleTypeDef hdma_tx;
    DMA_HandleTypeDef hdma_rx;
    SPI_HandleTypeDef SpiHandle;

    bool SPI_Bit_Order_Set;
    bool SPI_Data_Mode_Set;
    bool SPI_Clock_Divider_Set;
    bool SPI_Enabled;

    HAL_SPI_DMA_UserCallback SPI_DMA_UserCallback;
} SPI_State;

/*
 * SPI mapping
 * SPI1: NSS PA15; SCK PB3;  MISO PB4;  MOSI PB5;  AF5
 * SPI2: NSS PB12(D2); SCK PB13(D3); MISO PB14(D4); MOSI PB15(D5); AF5
 */
const STM32_SPI_Info spiMap[TOTAL_SPI] =
{
    { SPI2, DMA1_Channel4, DMA1_Channel5, DMA1_Channel4_IRQn, DMA1_Channel5_IRQn, GPIOB, GPIOB, GPIOB, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_12, GPIO_AF5_SPI2},
    { SPI1, DMA1_Channel2, DMA1_Channel3, DMA1_Channel2_IRQn, DMA1_Channel3_IRQn, GPIOB, GPIOB, GPIOB, GPIO_PIN_3,  GPIO_PIN_4,  GPIO_PIN_5,  GPIO_PIN_15, GPIO_AF5_SPI1}//sx1278 use
};

SPI_State spiState[TOTAL_SPI];
/* Private typedef -----------------------------------------------------------*/

/*
 * @brief Initialize the SPI peripheral and DMA.
 * @param spi: spi number chosed
 * @retral None
 */
void HAL_SPI_GPIO_DMA_Init(HAL_SPI_Interface spi)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock SCK MISO MOSI clock and SPI clock and DMA clock */
    if (spiMap[spi].SPI_Peripheral == SPI1) {
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
#ifdef useDMASPI
        //DMA2 clock
        __HAL_RCC_DMA1_CLK_ENABLE();
#endif
    } else if(spiMap[spi].SPI_Peripheral == SPI2) {
        __HAL_RCC_SPI2_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
#ifdef useDMASPI
        // DMA1 clock
        __HAL_RCC_DMA1_CLK_ENABLE();
#endif
    }
    /*##-2- Configure peripheral GPIO ##########################################*/
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = spiMap[spi].SPI_SCK_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;//GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = spiMap[spi].SPI_AF_Mapping;
    HAL_GPIO_Init( spiMap[spi].SPI_SCK_Port, &GPIO_InitStruct);
    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = spiMap[spi].SPI_MISO_Pin;
    GPIO_InitStruct.Alternate = spiMap[spi].SPI_AF_Mapping;
    HAL_GPIO_Init( spiMap[spi].SPI_MISO_Port, &GPIO_InitStruct);
    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = spiMap[spi].SPI_MOSI_Pin;
    GPIO_InitStruct.Alternate = spiMap[spi].SPI_AF_Mapping;
    HAL_GPIO_Init( spiMap[spi].SPI_MOSI_Port, &GPIO_InitStruct);

#ifdef useDMASPI
    /*##-3- Configure the DMA streams ##########################################*/
    /* Configure the DMA handler for Transmission process */
    spiState[spi].hdma_tx.Instance                 = spiMap[spi].SPI_TX_DMA_Stream;
    spiState[spi].hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    spiState[spi].hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    spiState[spi].hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    spiState[spi].hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    spiState[spi].hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    spiState[spi].hdma_tx.Init.Mode                = DMA_NORMAL;
    spiState[spi].hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
    HAL_DMA_Init(&spiState[spi].hdma_tx);

    /* Associate the initialized DMA handle to the the SPI handle */
    __HAL_LINKDMA(&spiState[spi].SpiHandle, hdmatx, spiState[spi].hdma_tx);

    /* Configure the DMA handler for Transmission process */
    spiState[spi].hdma_rx.Instance                 = spiMap[spi].SPI_RX_DMA_Stream;
    spiState[spi].hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    spiState[spi].hdma_rx.Init.PeriphInc           = DMA_PINC_ENABLE;
    spiState[spi].hdma_rx.Init.MemInc              = DMA_MINC_DISABLE;
    spiState[spi].hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    spiState[spi].hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    spiState[spi].hdma_rx.Init.Mode                = DMA_NORMAL;
    spiState[spi].hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&spiState[spi].hdma_rx);

    /* Associate the initialized DMA handle to the the SPI handle */
    __HAL_LINKDMA(&spiState[spi].SpiHandle, hdmarx, spiState[spi].hdma_rx);

    ///*##-4- Configure the NVIC for DMA #########################################*/
    ///* NVIC configuration for DMA transfer complete interrupt (SPI_TX) */
    //HAL_NVIC_SetPriority(spiMap[spi].SPI_DMA_TX_IRQn, 7, 1);
    //HAL_NVIC_EnableIRQ(spiMap[spi].SPI_DMA_TX_IRQn);

    ///* NVIC configuration for DMA transfer complete interrupt (SPI_RX) */
    //HAL_NVIC_SetPriority(spiMap[spi].SPI_DMA_RX_IRQn, 7, 0);
    //HAL_NVIC_EnableIRQ(spiMap[spi].SPI_DMA_RX_IRQn);
#endif
}

/*
 * @brief De-Initialize the SPI peripheral and DMA.
 * @param spi: spi number chosed
 * @retral None
 */
void HAL_SPI_GPIO_DMA_DeInit(HAL_SPI_Interface spi)
{
    /*##-1- Reset peripherals ##################################################*/
    if (spiMap[spi].SPI_Peripheral == SPI1) {
        __HAL_RCC_SPI1_FORCE_RESET();
        __HAL_RCC_SPI1_RELEASE_RESET();
    } else if(spiMap[spi].SPI_Peripheral == SPI2) {
        __HAL_RCC_SPI2_FORCE_RESET();
        __HAL_RCC_SPI2_RELEASE_RESET();
    }

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* Configure SPI SCK as alternate function  */
    HAL_GPIO_DeInit(spiMap[spi].SPI_SCK_Port, spiMap[spi].SPI_SCK_Pin);
    /* Configure SPI MISO as alternate function  */
    HAL_GPIO_DeInit(spiMap[spi].SPI_MISO_Port, spiMap[spi].SPI_MISO_Pin);
    /* Configure SPI MOSI as alternate function  */
    HAL_GPIO_DeInit(spiMap[spi].SPI_MOSI_Port, spiMap[spi].SPI_MOSI_Pin);

#ifdef useDMASPI
    /*##-3- Disable the DMA Streams ############################################*/
    /* De-Initialize the DMA Stream associate to transmission process */
    HAL_DMA_DeInit(&spiState[spi].hdma_tx);
    /* De-Initialize the DMA Stream associate to reception process */
    HAL_DMA_DeInit(&spiState[spi].hdma_rx);

    /*##-4- Disable the NVIC for DMA ###########################################*/
    HAL_NVIC_DisableIRQ(spiMap[spi].SPI_DMA_TX_IRQn);
    HAL_NVIC_DisableIRQ(spiMap[spi].SPI_DMA_RX_IRQn);
#endif
}

/*
 * @brief Initial the SPI, include setup which SPI used, and some flags.
 * @param spi: spi number chosed
 * @retral None
 */
void HAL_SPI_Initial(HAL_SPI_Interface spi)
{
    spiState[spi].SPI_Bit_Order_Set     = false;
    spiState[spi].SPI_Data_Mode_Set     = false;
    spiState[spi].SPI_Clock_Divider_Set = false;
    spiState[spi].SPI_Enabled           = false;
    spiState[spi].SpiHandle.Init.Mode   = SPI_MODE_MASTER;
}

/*
 * @brief Begin the SPI
 * @param spi: spi number chosed
 * @param pin: NSS pin, not used
 * @retral None
 */
void HAL_SPI_Begin(HAL_SPI_Interface spi, uint16_t pin)
{
    // Default to Master mode
    HAL_SPI_Begin_Ext(spi, SPI_MASTER, pin, NULL);
}

void HAL_SPI_Begin_Ext(HAL_SPI_Interface spi, SPI_Mode mode, uint16_t pin, void* reserved)
{
    if (pin == SPI_DEFAULT_SS) {
        pin = spiMap[spi].SPI_SS_Pin;
    }

    HAL_SPI_GPIO_DMA_Init(spi);
    /* SPI configuration */
    spiState[spi].SpiHandle.Instance            = spiMap[spi].SPI_Peripheral;
    if(SPI_MASTER == mode) {
        spiState[spi].SpiHandle.Init.Mode       = SPI_MODE_MASTER;
    } else {
        spiState[spi].SpiHandle.Init.Mode       = SPI_MODE_SLAVE;
    }
    spiState[spi].SpiHandle.Init.Direction      = SPI_DIRECTION_2LINES;
    spiState[spi].SpiHandle.Init.DataSize       = SPI_DATASIZE_8BIT; // SPI_DATASIZE_8BIT; // SPI_DATASIZE_16BIT
    spiState[spi].SpiHandle.Init.NSS            = SPI_NSS_SOFT;
    spiState[spi].SpiHandle.Init.TIMode         = SPI_TIMODE_DISABLE;
    spiState[spi].SpiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    spiState[spi].SpiHandle.Init.CRCPolynomial  = 7;

    if(spiState[spi].SPI_Data_Mode_Set != true) {
        //Default: SPI_MODE3
        HAL_SPI_Set_Data_Mode(spi, SPI_MODE3);
    }
    if(spiState[spi].SPI_Clock_Divider_Set != true) {
        HAL_SPI_Set_Clock_Divider(spi, SPI_BAUDRATEPRESCALER_16);
    }
    if(spiState[spi].SPI_Bit_Order_Set != true) {
        //Default: MSBFIRST
        HAL_SPI_Set_Bit_Order(spi, SPI_FIRSTBIT_MSB);
    }
    HAL_SPI_Init(&spiState[spi].SpiHandle);
    spiState[spi].SPI_Enabled = true;
    //TODO slave mode
}

/*
 * @brief End the SPI.
 * @param spi: spi number chosed
 * @retral None
 */
void HAL_SPI_End(HAL_SPI_Interface spi)
{
    HAL_SPI_GPIO_DMA_DeInit(spi);
    spiState[spi].SPI_Enabled = false;
}

/*
 * @brief Set the SPI bit order.
 * @param spi: spi number chosed.
 * @param order: LSBFIRST or MSBFIRST.
 * @retral None
 */
void HAL_SPI_Set_Bit_Order(HAL_SPI_Interface spi, uint8_t order)
{
    if(order == LSBFIRST) {
        spiState[spi].SpiHandle.Init.FirstBit = SPI_FIRSTBIT_LSB;
    } else {
        spiState[spi].SpiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    }
    spiState[spi].SPI_Bit_Order_Set = true;
}

/*
 * @brief Set the SPI data mode.
 * @param spi: spi number chosed.
 * @param mode: SPI mode.
 * @retral None
 */
void HAL_SPI_Set_Data_Mode(HAL_SPI_Interface spi, uint8_t mode)
{
    switch(mode) {
        case SPI_MODE0:
            spiState[spi].SpiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
            spiState[spi].SpiHandle.Init.CLKPhase   = SPI_PHASE_1EDGE;
            break;
        case SPI_MODE1:
            spiState[spi].SpiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
            spiState[spi].SpiHandle.Init.CLKPhase   = SPI_PHASE_2EDGE;
            break;
        case SPI_MODE2:
            spiState[spi].SpiHandle.Init.CLKPolarity = SPI_POLARITY_HIGH;
            spiState[spi].SpiHandle.Init.CLKPhase   = SPI_PHASE_1EDGE;
            break;
        case SPI_MODE3:
            spiState[spi].SpiHandle.Init.CLKPolarity = SPI_POLARITY_HIGH;
            spiState[spi].SpiHandle.Init.CLKPhase   = SPI_PHASE_2EDGE;
            break;
    }
    spiState[spi].SPI_Data_Mode_Set = true;
}

/*
 * @brief Set the SPI clock divider.
 * @param spi: spi number chosed.
 * @param order: spi baudrateprescaler.
 * @retral None
 */
void HAL_SPI_Set_Clock_Divider(HAL_SPI_Interface spi, uint8_t rate)
{
    spiState[spi].SpiHandle.Init.BaudRatePrescaler = rate;
    spiState[spi].SPI_Clock_Divider_Set = true;
}

int32_t HAL_SPI_Set_Settings(HAL_SPI_Interface spi, uint8_t set_default, uint8_t clockdiv, uint8_t order, uint8_t mode, void* reserved)
{
#if 0
    if (!set_default) {
        HAL_SPI_Set_Clock_Divider_Impl(spi, clockdiv);
        HAL_SPI_Set_Bit_Order_Impl(spi, order);
        HAL_SPI_Set_Data_Mode_Impl(spi, mode);
    }

    spiState[spi].SPI_Clock_Divider_Set = !set_default;
    spiState[spi].SPI_Data_Mode_Set = !set_default;
    spiState[spi].SPI_Bit_Order_Set = !set_default;

    if (set_default) {
        // HAL_SPI_End(spi);
        HAL_SPI_Begin_Ext(spi, spiState[spi].mode, spiState[spi].SPI_SS_Pin, NULL);
    } else if (spiState[spi].SPI_Enabled != false) {
        SPI_Cmd(spiMap[spi].SPI_Peripheral, DISABLE);
        SPI_Init(spiMap[spi].SPI_Peripheral, &spiState[spi].SPI_InitStructure);
        SPI_Cmd(spiMap[spi].SPI_Peripheral, ENABLE);
    }
#endif
    return 0;
}

/*
 * @brief SPI send and recieve data.
 * @param spi: spi number chosed.
 * @param data: The data to be transmitted.
 * @retral The received data.
 */
uint16_t HAL_SPI_Send_Receive_Data(HAL_SPI_Interface spi, uint16_t data)
{
    uint8_t dataTrans = data;
    uint8_t rxDataTrans = 0;

    if (spiState[spi].SpiHandle.Init.Mode == SPI_MODE_SLAVE) {
        return 0;
    }
#ifdef useDMASPI
    HAL_SPI_TransmitReceive_DMA(&spiState[spi].SpiHandle, &dataTrans, &rxDataTrans, 1);
#else
    HAL_SPI_TransmitReceive(&spiState[spi].SpiHandle, &dataTrans, &rxDataTrans, 1, 5);
#endif
    return rxDataTrans;
}

/*
 * @brief Check whether SPI is enabled or not.
 * @param spi: spi number chosed.
 * @retral The enabled flag: true, spi have begin; false, the spi not working.
 */
bool HAL_SPI_Is_Enabled(HAL_SPI_Interface spi)
{
    return spiState[spi].SPI_Enabled;
}

// TODO
void HAL_SPI_DMA_Transfer(HAL_SPI_Interface spi, void* tx_buffer, void* rx_buffer, uint32_t length, HAL_SPI_DMA_UserCallback userCallback)
{
#if 0
    spiState[spi].SPI_DMA_UserCallback = userCallback;
    HAL_SPI_TransmitReceive_DMA(&spiState[spi].SpiHandle, tx_buffer, rx_buffer, length);
#endif
}

void HAL_SPI_Info(HAL_SPI_Interface spi, hal_spi_info_t* info, void* reserved)
{
}

void HAL_SPI_Set_Callback_On_Select(HAL_SPI_Interface spi, HAL_SPI_Select_UserCallback cb, void* reserved)
{
}

void HAL_SPI_DMA_Transfer_Cancel(HAL_SPI_Interface spi)
{
}

int32_t HAL_SPI_DMA_Transfer_Status(HAL_SPI_Interface spi, HAL_SPI_TransferStatus* st)
{
    return 0;
}

#if 0
#ifdef useDMASPI
/**
 * @brief  This function handles SPI1 DMA Rx interrupt request.
 * @param  None
 * @retval None
 */
void DMA2_Stream0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(spiState[SPI2_D4_D5_D6].SpiHandle.hdmarx);
}

/**
 * @brief  This function handles SPI1 DMA Tx interrupt request.
 * @param  None
 * @retval None
 */
void DMA2_Stream5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(spiState[SPI1_A5_A6_A7].SpiHandle.hdmatx);
}

/**
 * @brief  This function handles SPI2 DMA Rx interrupt request.
 * @param  None
 * @retval None
 */
void DMA1_Stream0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(spiState[SPI3_D3_D2_D1].SpiHandle.hdmarx);
}

/**
 * @brief  This function handles SPI2 DMA Tx interrupt request.
 * @param  None
 * @retval None
 */
void DMA1_Stream5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(spiState[SPI3_D3_D2_D1].SpiHandle.hdmatx);
}
#endif
#endif

