cmake_minimum_required(VERSION 3.10)
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs" CACHE INTERNAL "")
SET(CMAKE_CROSSCOMPILING 1)
#set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
#set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
project(src)

set(CMAKE_C_STANDARD 11)

include_directories(inc)
include_directories(stm/Libraries/CMSIS/Core/CM3)
include_directories(stm/Libraries/STM32_USB-FS-Device_Driver/inc)
include_directories(stm/Libraries/STM32F10x_StdPeriph_Driver/inc)

set (RUN_MODE ROM_RUN)
set (MCU cortex-m3)
set (SUBMDL stm32f103)
##USE_THUMB_MODE = YES
#THUMB    = -mthumb
set (VID 0x20a0)
set (PID 0x4108)

add_definitions(-D${RUN_MODE} -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -DUSE_STM3210E_EVAL -DGLOBAL_VID=${VID} -DGLOBAL_PID=${PID})
#
#CFLAGS = -g$(DEBUG)
#CFLAGS += $(CDEFS) $(CINCS)
#CFLAGS += -O$(OPT)
#CFLAGS += -Wall -Wcast-align -Wimplicit
#CFLAGS += -Wpointer-arith -Wswitch
#CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
#CFLAGS += -Wa,-adhlns=$(subst $(suffix $<),.lst,$<)
#CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
#CFLAGS += -Wcast-qual
#CONLYFLAGS += -Wmissing-prototypes
#CONLYFLAGS += -Wstrict-prototypes
#CONLYFLAGS += -Wmissing-declarations
set(CFLAGS )
set(GENDEPFLAGS )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=${MCU} ${THUMB_IW} -I. ${CFLAGS} ${GENDEPFLAGS}")
#set (LDFLAGS -Tstm32.ld)
#link_libraries( ${LDFLAGS} )
#ASFLAGS = $(ADEFS) -Wa,-adhlns=$(<:.S=.lst),-g$(DEBUG)

#-mcpu=$(MCU)

add_executable(src
        ccid/CCIDHID_USB/CCIDHID_usb_desc.c
        ccid/CCIDHID_USB/CCIDHID_usb_prop.c
        ccid/smartcard/smartcard.c
        ccid/Ccid_usb.c
        ccid/CcidLocalAccess.c
        ccid/Crd.c
        ccid/Ifd_ccid.c
        ccid/Ifd_protocol.c
        crypt/aes/aes.c
        crypt/sha1/hmac-sha1.c
        crypt/sha1/sha1.c
        hotp/hotp.c
        inc/AccessInterface.h
        inc/aes.h
        inc/CCID_Crd.h
        inc/CCID_Global.h
        inc/CCID_Ifd_ccid.h
        inc/CCID_Ifd_protocol.h
        inc/CCID_Macro.h
        inc/CCID_SlotErrorCode.h
        inc/CCID_usb.h
        inc/CCID_usb_conf.h
        inc/CCID_usb_desc.h
        inc/CCID_usb_prop.h
        inc/CCIDHID_usb.h
        inc/CCIDHID_usb_conf.h
        inc/CCIDHID_usb_desc.h
        inc/CCIDHID_usb_prop.h
        inc/CcidLocalAccess.h
        inc/delays.h
        inc/FAT12.h
        inc/FlashStorage.h
        inc/fsmc_nand.h
        inc/HandleAesStorageKey.h
        inc/hmac-sha1.h
        inc/hotp.h
        inc/hw_config.h
        inc/keyboard.h
        inc/mass_mal.h
        inc/memory.h
        inc/memory_ops.h
        inc/msd.h
        inc/nand_if.h
        inc/password_safe.h
        inc/platform_config.h
        inc/ramdisk.h
        inc/RAMDISK_usb_desc.h
        inc/RAMDISK_usb_prop.h
        inc/report_protocol.h
        inc/sdcard.h
        inc/sha1.h
        inc/smartcard.h
        inc/stm32f10x_conf.h
        inc/stm32f10x_it.h
        inc/stm32f10x_systick.h
        inc/test_code.h
        inc/type.h
        inc/usb_bot.h
        inc/usb_conf.h
        inc/usb_desc.h
        inc/usb_istr.h
        inc/usb_prop.h
        inc/usb_pwr.h
        inc/usb_scsi.h
        keyboard/keyboard.c
        keyboard/report_protocol.c
        pwd-safe/FlashStorage.c
        pwd-safe/HandleAesStorageKey.c
        pwd-safe/password_safe.c
        sd-disk/sd-usb/usb_desc.c
        sd-disk/sd-usb/usb_prop.c
        stm/Libraries/CMSIS/Core/CM3/startup/gcc/startup_stm32f10x_hd.c
        stm/Libraries/CMSIS/Core/CM3/startup/gcc/startup_stm32f10x_ld.c
        stm/Libraries/CMSIS/Core/CM3/startup/gcc/startup_stm32f10x_md.c
        stm/Libraries/CMSIS/Core/CM3/core_cm3.c
        stm/Libraries/CMSIS/Core/CM3/core_cm3.h
        stm/Libraries/CMSIS/Core/CM3/stm32f10x.h
        stm/Libraries/CMSIS/Core/CM3/system_stm32f10x.c
        stm/Libraries/CMSIS/Core/CM3/system_stm32f10x.h
        stm/Libraries/STM32_USB-FS-Device_Driver/inc/usb_core.h
        stm/Libraries/STM32_USB-FS-Device_Driver/inc/usb_def.h
        stm/Libraries/STM32_USB-FS-Device_Driver/inc/usb_init.h
        stm/Libraries/STM32_USB-FS-Device_Driver/inc/usb_int.h
        stm/Libraries/STM32_USB-FS-Device_Driver/inc/usb_lib.h
        stm/Libraries/STM32_USB-FS-Device_Driver/inc/usb_mem.h
        stm/Libraries/STM32_USB-FS-Device_Driver/inc/usb_regs.h
        stm/Libraries/STM32_USB-FS-Device_Driver/inc/usb_type.h
        stm/Libraries/STM32_USB-FS-Device_Driver/src/usb_core.c
        stm/Libraries/STM32_USB-FS-Device_Driver/src/usb_init.c
        stm/Libraries/STM32_USB-FS-Device_Driver/src/usb_int.c
        stm/Libraries/STM32_USB-FS-Device_Driver/src/usb_mem.c
        stm/Libraries/STM32_USB-FS-Device_Driver/src/usb_regs.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/misc.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_adc.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_bkp.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_can.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_crc.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_dac.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_dbgmcu.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_dma.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_exti.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_flash.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_fsmc.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_gpio.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_i2c.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_iwdg.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_pwr.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_rcc.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_rtc.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_sdio.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_spi.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_tim.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_usart.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_wwdg.h
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/misc.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_adc.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_bkp.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_can.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_crc.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dac.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dbgmcu.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dma.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_fsmc.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_i2c.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_iwdg.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_pwr.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rtc.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_sdio.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c
        stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_wwdg.c
        stm/stm32f10x_it.c
        stm/stm32f10x_systick.c
        usb/usb_bot.c
        usb/usb_endp.c
        usb/usb_istr.c
        usb/usb_pwr.c
        utils/delays.c
        utils/memory_ops.c
        hw_config.c
        main.c
        test_code.c)
