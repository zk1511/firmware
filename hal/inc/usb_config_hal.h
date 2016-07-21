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

#ifndef USB_CONFIG_HAL_H
#define	USB_CONFIG_HAL_H

/* We are temporary defining this macro over here */
/* This could also be passed via -D option in build script */
#define INTOROBOT_USB_SERIAL        //Default is Virtual COM Port
//#define INTOROBOT_USB_MOUSE
//#define INTOROBOT_USB_KEYBOARD

#if !defined (INTOROBOT_USB_SERIAL) && !defined (INTOROBOT_USB_MOUSE) && !defined (INTOROBOT_USB_KEYBOARD)
#define USB_CDC_ENABLE	//Use USB Serial feature by default if none is defined
#elif defined (INTOROBOT_USB_SERIAL)
#define USB_CDC_ENABLE	//Enable USB CDC code
#elif defined (INTOROBOT_USB_MOUSE) || defined (INTOROBOT_USB_KEYBOARD)
#define USB_HID_ENABLE	//Enable USB HID code
#endif


#endif	/* USB_CONFIG_HAL_H */
