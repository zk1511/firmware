/**
 ******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

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
#include "pwm_hal.h"


void HAL_PWM_Write(uint16_t pin, uint8_t value)
{
}

void HAL_PWM_Write_With_Frequency(uint16_t pin, uint8_t value, uint16_t pwm_frequency)
{
}

void HAL_PWM_Write_Ext(uint16_t pin, uint32_t value)
{
}

void HAL_PWM_Write_With_Frequency_Ext(uint16_t pin, uint32_t value, uint32_t pwm_frequency)
{
}

uint16_t HAL_PWM_Get_Frequency(uint16_t pin)
{
    return 0;
}

uint16_t HAL_PWM_Get_AnalogValue(uint16_t pin)
{
    return 0;
}

uint32_t HAL_PWM_Get_Frequency_Ext(uint16_t pin)
{
    return 0;
}

uint32_t HAL_PWM_Get_AnalogValue_Ext(uint16_t pin)
{
    return 0;
}

uint32_t HAL_PWM_Get_Max_Frequency(uint16_t pin)
{
    return 0;
}

uint8_t HAL_PWM_Get_Resolution(uint16_t pin)
{
   return 0;
}

uint8_t HAL_PWM_Timer_Resolution(uint16_t pin)
{
    return 16;
}

void HAL_PWM_Set_Resolution(uint16_t pin, uint8_t resolution)
{
}

