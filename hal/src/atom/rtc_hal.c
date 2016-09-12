/**
 ******************************************************************************
 * @file    rtc_hal.c
 * @author  Satish Nair, Brett Walach
 * @version V1.0.0
 * @date    12-Sept-2014
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

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
#include "rtc_hal.h"

RTC_HandleTypeDef RtcHandle;

void HAL_RTC_Initial(void)
{
#if 0
    /*##-1- Configure the RTC peripheral #######################################*/
    /* Configure RTC prescaler and RTC data registers */
    /* RTC configured as follows:
       - Hour Format    = Format 24
       - Asynch Prediv  = Value according to source clock
       - Synch Prediv   = Value according to source clock
       - OutPut         = Output Disable
       - OutPutPolarity = High Polarity
       - OutPutType     = Open Drain */
    RtcHandle.Instance            = RTC;
    RtcHandle.Init.HourFormat     = RTC_HOURFORMAT_24;
    RtcHandle.Init.AsynchPrediv   = 0x7F; //RTC_ASYNCH_PREDIV; //0x7F;
    RtcHandle.Init.SynchPrediv    = 0x00FF; //RTC_SYNCH_PREDIV;  //0x00FF;
    RtcHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
    RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RtcHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;

    if (HAL_RTC_Init(&RtcHandle) != HAL_OK)
        {
            DEBUG("RTC Init Error!");
        }
    RTC_CalendarAlarmConfig();

#endif
}

time_t HAL_RTC_Get_UnixTime(void)
{
    return 0;
}

void HAL_RTC_Set_UnixTime(time_t value)
{
    #if 0
    struct tm *tmTemp = gmtime( &value );
    RTC_DateTypeDef sdatestructure;
    RTC_TimeTypeDef stimestructure;

    /*##-1- Configure the Date #################################################*/
    /* Set Date: Friday January 1st 2016 */
    sdatestructure.Year    = dec2hex_direct(tmTemp->tm_year + 1900 -2000);
    sdatestructure.Month   = dec2hex_direct(tmTemp->tm_mon + 1);
    sdatestructure.Date    = dec2hex_direct(tmTemp->tm_mday);
    sdatestructure.WeekDay = RTC_WEEKDAY_FRIDAY;

    if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
    {
        DEBUG("RTC Set_UnixTime SetDate failed!");
    }

    /*##-2- Configure the Time #################################################*/
    /* Set Time: 00:00:00 */
    stimestructure.Hours          = dec2hex_direct(tmTemp->tm_hour);
    stimestructure.Minutes        = dec2hex_direct(tmTemp->tm_min);
    stimestructure.Seconds        = dec2hex_direct(tmTemp->tm_sec);
    stimestructure.TimeFormat     = RTC_HOURFORMAT12_AM;
    stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
    stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

    if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
    {
        DEBUG("RTC Set_UnixTime SetTime failed!");
    }

    /*##-3- Writes a data in a RTC Backup data Register1 #######################*/
    HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);


#endif
}

void HAL_RTC_Set_Alarm(uint32_t value)
{
}

void HAL_RTC_Set_UnixAlarm(time_t value)
{
    #if 0
    HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
    RTC_AlarmTypeDef salarmstructure;

    time_t alarm_time = HAL_RTC_Get_UnixTime() + value;
    struct tm *tmTemp = gmtime( &value );


    /*##-- Configure the RTC Alarm peripheral #################################*/
    /* Set Alam to 00:00:20
       RTC Alarm Generation: Alarm on Hours, Minutes and Seconds */
    salarmstructure.Alarm                = RTC_ALARM_A;
    salarmstructure.AlarmDateWeekDay     = RTC_WEEKDAY_FRIDAY;
    salarmstructure.AlarmDateWeekDaySel  = RTC_ALARMDATEWEEKDAYSEL_DATE;
    salarmstructure.AlarmMask            = RTC_ALARMMASK_DATEWEEKDAY;
   // salarmstructure.AlarmSubSecondMask   = RTC_ALARMSUBSECONDMASK_NONE;
    salarmstructure.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
    salarmstructure.AlarmTime.Hours      = dec2hex_direct(tmTemp->tm_hour);
    salarmstructure.AlarmTime.Minutes    = dec2hex_direct(tmTemp->tm_min);
    salarmstructure.AlarmTime.Seconds    = dec2hex_direct(tmTemp->tm_sec);
   // salarmstructure.AlarmTime.SubSeconds = 0x00;

    if(HAL_RTC_SetAlarm_IT(&RtcHandle,&salarmstructure,RTC_FORMAT_BCD) != HAL_OK)
    {
        /* Initialization Error */
        DEBUG("RTC CalendarAlarmConfig SetAlarm Error!");
    }

#endif
}

void HAL_RTC_Cancel_UnixAlarm(void)
{
    /* HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn); */
}