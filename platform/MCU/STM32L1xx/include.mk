TARGET_CMSIS=$(PLATFORM_MCU_PATH)/CMSIS
INCLUDE_DIRS += $(TARGET_CMSIS)/Include
INCLUDE_DIRS += $(TARGET_CMSIS)/Device/ST/STM32L1xx/Include

TARGET_INTOROBOT_PATH = $(PLATFORM_MCU_PATH)/IntoRobot_Firmware_Driver
INCLUDE_DIRS += $(TARGET_INTOROBOT_PATH)/inc

TARGET_STM32L1XX_HAL_PATH = $(PLATFORM_MCU_PATH)/STM32L1xx_HAL_Driver
INCLUDE_DIRS += $(TARGET_STM32L1XX_HAL_PATH)/Inc

TARGET_USB_FS_PATH = $(PLATFORM_MCU_PATH)/STM32_USB_Device_Library

# Add include to all objects built for this target
INCLUDE_DIRS += $(TARGET_USB_FS_PATH)/Class/CDC/Inc
INCLUDE_DIRS += $(TARGET_USB_FS_PATH)/Class/DFU/Inc
INCLUDE_DIRS += $(TARGET_USB_FS_PATH)/Core/Inc

