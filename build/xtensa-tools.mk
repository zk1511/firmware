#
# 定义xtensa编译工具相关参数
#

# 定义编译器和工具的前缀
GCC_ARM_PATH ?= $(PROJECT_ROOT)/tools/xtensa-lx106-elf/bin/
GCC_PREFIX ?= xtensa-lx106-elf-

include $(COMMON_BUILD)/common-tools.mk


# C 编译参数
ifeq ("$(MODULE)","bootloader")
#CFLAGS += -O0 -g -mno-text-section-literals
CFLAGS += -O0 -g -mtext-section-literals -D__ets__ -DICACHE_FLASH
else
CFLAGS += -Os -g -mtext-section-literals -ffunction-sections -fdata-sections -DICACHE_FLASH
endif
CFLAGS += -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -falign-functions=4 -MMD -std=gnu99

# C++ 编译参数
CPPFLAGS += -fno-exceptions -fno-rtti -std=c++11

ASFLAGS += -c -g -x assembler-with-cpp -MMD


# Board definitions
FLASH_SIZE ?= 16M    #此处必须是16M  此时用户参数区才会在1M 的最后16K上
FLASH_MODE ?= qio
FLASH_SPEED ?= 40

# Upload parameters
UPLOAD_SPEED ?= 230400

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    UPLOAD_PORT ?= /dev/ttyACM0
endif
ifeq ($(UNAME_S),Darwin)
    UPLOAD_PORT ?= /dev/cu.usbmodem1411
endif

#UPLOAD_SPEED ?= 921600
#UPLOAD_PORT ?= /dev/cu.SLAB_USBtoUART
UPLOAD_VERB ?= -v
UPLOAD_RESET ?= nodemcu

#
