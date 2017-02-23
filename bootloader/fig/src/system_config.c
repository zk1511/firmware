#include "hw_config.h"
#include "flash_map.h"
#include "rtc.h"
#include "sdkconfig.h"
#include "esp_image_format.h"
#include "rom/spi_flash.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/timer_group_reg.h"


/**
 * @brief  System Clock Configuration
 * @param  96M
 * @retval None
 */
void SystemClock_Config(void)
{
    rtc_set_cpu_freq(CPU_XTAL);
    rtc_set_cpu_freq(CPU_80M);
}
/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
    HAL_UI_SysTick_Handler();
}

/**
 * @brief  System Clock Configuration
 * @param  96M
 * @retval None
 */
void HwTimer_config(void)
{

}

static void uart_console_configure(void)
{
    const int uart_num = CONFIG_CONSOLE_UART_NUM;

    uartAttach();
    ets_install_uart_printf();

    // ROM bootloader may have put a lot of text into UART0 FIFO.
    // Wait for it to be printed.
    uart_tx_wait_idle(0);

    // Set configured UART console baud rate
    const int uart_baud = CONFIG_CONSOLE_UART_BAUDRATE;
    uart_div_modify(uart_num, (APB_CLK_FREQ << 4) / uart_baud);
}

/**
 * @brief  update flash config
 * @param  96M
 * @retval None
 */
static void Update_flash_config(void)
{
    esp_image_header_t fhdr;
    uint32_t size;

    if(esp_image_load_header(APP_ADDR, true, &fhdr) != ESP_OK) {
        return;
    }
    SPIUnlock();

    switch(fhdr.spi_size) {
        case ESP_IMAGE_FLASH_SIZE_1MB:
            size = 1;
            break;
        case ESP_IMAGE_FLASH_SIZE_2MB:
            size = 2;
            break;
        case ESP_IMAGE_FLASH_SIZE_4MB:
            size = 4;
            break;
        case ESP_IMAGE_FLASH_SIZE_8MB:
            size = 8;
            break;
        case ESP_IMAGE_FLASH_SIZE_16MB:
            size = 16;
            break;
        default:
            size = 2;
    }
    Cache_Read_Disable( 0 );
    // Set flash chip size
    SPIParamCfg(g_rom_flashchip.deviceId, size * 0x100000, 0x10000, 0x1000, 0x100, 0xffff);
    // TODO: set mode
    // TODO: set frequency
    Cache_Flush(0);
    Cache_Read_Enable( 0 );
}

/**
 * @brief  Configures Main system clocks & power.
 * @param  None
 * @retval None
 */
void Set_System(void)
{
    SystemClock_Config();
    uart_console_configure();
    /* disable watch dog here */
    REG_CLR_BIT( RTC_CNTL_WDTCONFIG0_REG, RTC_CNTL_WDT_FLASHBOOT_MOD_EN );
    REG_CLR_BIT( TIMG_WDTCONFIG0_REG(0), TIMG_WDT_FLASHBOOT_MOD_EN );
    Update_flash_config();

    HwTimer_config();
    //HAL_UI_Initial();
}

void delay(uint32_t ms)
{
    ets_delay_us(ms*1000);
}

void System_Reset(void)
{
    _ResetVector();
}

/* empty rtc_printf implementation, to work with librtc
   linking. Can be removed once -lrtc is removed from bootloader's
   main component.mk.
*/
int rtc_printf(void)
{
    return 0;
}
