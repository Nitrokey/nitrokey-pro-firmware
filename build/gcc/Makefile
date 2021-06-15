
#
# WinARM template makefile 
# by Martin Thomas, Kaiserslautern, Germany 
# <eversmith@heizung-thomas.de>
#
# based on the WinAVR makefile written by Eric B. Weddington, Jörg Wunsch, et al.
# Released to the Public Domain
# Please read the make user manual!
#
#
# On command line:
#
# make all = Make software.
#
# make clean = Clean out built project files.
#
# make program = Download the hex file to the device
#
# (TODO: make filename.s = Just compile filename.c into the assembler code only)
#
# To rebuild project do "make clean" then "make all".
#
# Changelog:
# - 22. Feb. 2006  - added AT91LIBNOWARN setting (mth)
# - 17. Feb. 2005  - added thumb-interwork support (mth)
# - 28. Apr. 2005  - added C++ support (mth)
# - 29. Arp. 2005  - changed handling for lst-Filename (mth)
# -  1. Nov. 2005  - exception-vector placement options (mth)
# - 15. Nov. 2005  - added library-search-path (EXTRA_LIB...) (mth)
# -  2. Dec. 2005  - fixed ihex and binary file extensions (mth)
#


# Cmd interface paramenters
VID?=0x20a0
PID?=0x4108
HW_REV?=4
BUILD_DEBUG=0

###########################################
# THUMB, RAM_RUN and Remapping currently
# not supported or not tested in this 
# ADuC example
###########################################


# MCU name and submodel
MCU      = cortex-m3
SUBMDL   = stm32f103
##USE_THUMB_MODE = YES
THUMB    = -mthumb
#THUMB_IW = -mthumb-interwork

## Create ROM-Image (final)
RUN_MODE=ROM_RUN
## Create RAM-Image (debugging)
#RUN_MODE=RAM_RUN

## not used in this example:
## Exception-Vector placement only supported for "ROM_RUN"
## (placement settings ignored when using "RAM_RUN")
## - Exception vectors in ROM:
#VECTOR_LOCATION=VECTORS_IN_ROM
## - Exception vectors in RAM:
#VECTOR_LOCATION=VECTORS_IN_RAM


# Target file name (without extension).
TARGET = nitrokey-pro-firmware-hw$(HW_REV)

# List C source files here. (C dependencies are automatically generated.)
# use file-extension c for "c-only"-files
SRC = 		../../src/main.c                                         \
			../../src/hw_config.c                                         \
			../../src/test_code.c                                         \
			../../src/utils/delays.c	\
			../../src/utils/memory_ops.c			\
			../../src/ccid/Ccid_usb.c                                         \
			../../src/ccid/Ifd_protocol.c                                         \
			../../src/ccid/Crd.c                                         \
			../../src/ccid/Ifd_ccid.c                                         \
			../../src/ccid/CcidLocalAccess.c                                         \
			../../src/ccid/smartcard/smartcard.c                                         \
			../../src/ccid/CCIDHID_USB/CCIDHID_usb_prop.c                                         \
			../../src/ccid/CCIDHID_USB/CCIDHID_usb_desc.c                                         \
			../../src/sd-disk/sd-usb/usb_desc.c                                         \
			../../src/sd-disk/sd-usb/usb_prop.c                                         \
			../../src/stm/stm32f10x_systick.c                                         \
			../../src/stm/stm32f10x_it.c                                         \
			../../src/stm/Libraries/CMSIS/Core/CM3/core_cm3.c                                         \
			../../src/stm/Libraries/CMSIS/Core/CM3/system_stm32f10x.c                                         \
			../../src/stm/Libraries/CMSIS/Core/CM3/startup/gcc/startup_stm32f10x_hd.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/misc.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_adc.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_bkp.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_can.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_crc.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dac.c        \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dbgmcu.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dma.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_fsmc.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_i2c.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_iwdg.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_pwr.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rtc.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_sdio.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c                                         \
			../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_wwdg.c                                         \
			../../src/stm/Libraries/STM32_USB-FS-Device_Driver/src/usb_core.c                                         \
			../../src/stm/Libraries/STM32_USB-FS-Device_Driver/src/usb_init.c                                         \
			../../src/stm/Libraries/STM32_USB-FS-Device_Driver/src/usb_int.c                                         \
			../../src/stm/Libraries/STM32_USB-FS-Device_Driver/src/usb_mem.c                                         \
			../../src/stm/Libraries/STM32_USB-FS-Device_Driver/src/usb_regs.c                                         \
			../../src/crypt/aes/aes.c                                         \
			../../src/crypt/sha1/sha1.c                                         \
			../../src/crypt/sha1/hmac-sha1.c                                         \
			../../src/crypt/sha4/sha4.c                                         \
			../../src/crypt/pbkdf2/pbkdf2.c                                         \
			../../src/hotp/hotp.c                                         \
			../../src/keyboard/keyboard.c                                         \
			../../src/keyboard/report_protocol.c                                         \
			../../src/usb/usb_endp.c                                         \
			../../src/usb/usb_bot.c                                         \
			../../src/usb/usb_pwr.c                                         \
			../../src/usb/usb_istr.c                                         \
			../../src/pwd-safe/FlashStorage.c	\
			../../src/pwd-safe/HandleAesStorageKey.c	\
			../../src/pwd-safe/password_safe.c	\

			
# List C source files here which must be compiled in ARM-Mode.
# use file-extension c for "c-only"-files
SRCARM = 

# List C++ source files here.
# use file-extension cpp for C++-files (use extension .cpp)
CPPSRC = 

# List C++ source files here which must be compiled in ARM-Mode.
# use file-extension cpp for C++-files (use extension .cpp)
#CPPSRCARM = $(TARGET).cpp
CPPSRCARM = 

# List Assembler source files here.
# Make them always end in a capital .S.  Files ending in a lowercase .s
# will not be considered source files but generated files (assembler
# output from the compiler), and will be deleted upon "make clean"!
# Even though the DOS/Win* filesystem matches both .s and .S the same,
# it will preserve the spelling of the filenames, and gcc itself does
# care about how the name is spelled on its command-line.
ASRC = 

# List Assembler source files here which must be assembled in ARM-Mode..
ASRCARM = 
#./RVMDK/startup_stm32f10x_hd.s  \


## Using the Atmel AT91_lib produces warning with
## the default warning-levels. 
## yes - disable these warnings; no - keep default settings
#AT91LIBNOWARN = yes
AT91LIBNOWARN = no

## Output format. (can be ihex or binary)
FORMAT = ihex
#FORMAT = binary

# Optimization level, can be [0, 1, 2, 3, s]. 
# 0 = turn off optimization. s = optimize for size.
# (Note: 3 is not always the best optimization level. See avr-libc FAQ.)

ifeq ($(BUILD_DEBUG), 1)
$(warning "Running development build")
OPT = 0
else
OPT = 2
endif

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
#DEBUG = stabs
DEBUG = dwarf-2


# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
EXTRAINCDIRS = ../../src/inc                                             \
               ../../src/stm/Libraries/CMSIS/Core/CM3                    \
			   ../../src/stm/Libraries/STM32_USB-FS-Device_Driver/inc    \
			   ../../src/stm/Libraries/STM32F10x_StdPeriph_Driver/inc


# List any extra directories to look for library files here.
#     Each directory must be seperated by a space.
#EXTRA_LIBDIRS = ../arm7_efsl_0_2_4
EXTRA_LIBDIRS = 


# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99

# Place -D or -U options for C here
CDEFS =  -D$(RUN_MODE) -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -DUSE_STM3210E_EVAL -DGLOBAL_VID=$(VID) -DGLOBAL_PID=$(PID) -DHW_REV=$(HW_REV)
#CDEFS =  -D$(RUN_MODE) -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -DUSE_BOARD_STICK_V12
# Place -I options here
CINCS =

# Place -D or -U options for ASM here
ADEFS =  -D$(RUN_MODE)

ifdef VECTOR_LOCATION
CDEFS += -D$(VECTOR_LOCATION)
ADEFS += -D$(VECTOR_LOCATION)
endif

# Compiler flags.
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
#
# Flags for C and C++ (arm-elf-gcc/arm-elf-g++)
CFLAGS = -g$(DEBUG)
CFLAGS += $(CDEFS) $(CINCS)
CFLAGS += -O$(OPT)
CFLAGS_OUR_SRC += -Werror
# CFLAGS_OUR_SRC += -Wextra
CFLAGS += -Wall -Wcast-align -Wimplicit
CFLAGS += -Wpointer-arith -Wswitch
CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
CFLAGS += -Wa,-adhlns=$(subst $(suffix $<),.lst,$<)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += -Wno-cast-qual -Wno-cast-align

# flags only for C
CONLYFLAGS += -Wnested-externs 
CONLYFLAGS += $(CSTANDARD)

ifneq ($(AT91LIBNOWARN),yes)
#AT91-lib warnings with:
CFLAGS += -Wcast-qual
CONLYFLAGS += -Wmissing-prototypes
CONLYFLAGS += -Wstrict-prototypes
CONLYFLAGS += -Wmissing-declarations
endif

CFLAGS += -Wno-cast-qual
# extra diagnostic
#CFLAGS += -Wconversion -fno-common -Wdouble-promotion -Wshadow -Wpadded
#CFLAGS += -fstack-usage & -Wstack-usage=
ifeq ($(BUILD_DEBUG), 1)
#CFLAGS += -fdata-sections # makes CCID not working
CFLAGS += -ffunction-sections
#CFLAGS += -fstack-protector-all
endif
#CFLAGS += -funwind-tables
#CFLAGS += -fno-unwind-tables -fno-exceptions


# flags only for C++ (arm-elf-g++)
# CPPFLAGS = -fno-rtti -fno-exceptions
CPPFLAGS = 

# Assembler flags.
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -ahlms:    create listing
#  -g$(DEBUG):have the assembler create line number information
ASFLAGS = $(ADEFS) -Wa,-adhlns=$(<:.S=.lst),-g$(DEBUG)


#Additional libraries.

# Extra libraries
#    Each library-name must be seperated by a space.
#    To add libxyz.a, libabc.a and libefsl.a: 
#    EXTRA_LIBS = xyz abc efsl
#EXTRA_LIBS = efsl
EXTRA_LIBS =

#Support for newlibc-lpc (file: libnewlibc-lpc.a)
#NEWLIBLPC = -lnewlib-lpc

MATH_LIB = -lm

# CPLUSPLUS_LIB = -lstdc++


# Linker flags.
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -nostartfiles -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += -lc
LDFLAGS += $(NEWLIBLPC) $(MATH_LIB)
LDFLAGS += -lc -lgcc 
LDFLAGS += -lrdimon
LDFLAGS += $(CPLUSPLUS_LIB)
LDFLAGS += $(patsubst %,-L%,$(EXTRA_LIBDIRS))
LDFLAGS += $(patsubst %,-l%,$(EXTRA_LIBS))
LDFLAGS += -Wl,--print-memory-usage
LDFLAGS += -Wl,--print-gc-sections

ifeq ($(BUILD_DEBUG), 1)
LDFLAGS += -Wl,--gc-sections
endif

# Set Linker-Script Depending On Selected Memory and Controller
LDFLAGS +=-Tstm32.ld
#ifeq ($(RUN_MODE),RAM_RUN)
#LDFLAGS +=-T$(SUBMDL)-RAM.ld
#else 
#LDFLAGS +=-T$(SUBMDL)-ROM.ld
#endif

ifeq ($(BUILD_DEBUG), 1)
LDFLAGS +=-TSTM32_SEC_FLASH-debug.ld
else
LDFLAGS +=-TSTM32_SEC_FLASH.ld
endif

# ---------------------------------------------------------------------------
# Flash-Programming support using lpc21isp by Martin Maurer 
# only for Philips LPC and Analog ADuC ARMs
#
# Settings and variables:
#LPC21ISP = lpc21isp
LPC21ISP = lpc21isp
LPC21ISP_PORT = com2
LPC21ISP_BAUD = 9600
# XTAL-Parameter seems to be a dummy for ADuC 
# but is needed or lpc21isp will stop with an error message
LPC21ISP_XTAL = 32
LPC21ISP_FLASHFILE = $(TARGET).hex
# verbose output:
## LPC21ISP_DEBUG = -debug
# enter bootloader via RS232 DTR/RTS (only if hardware supports this
# feature - see Philips AppNote):
#LPC21ISP_OPTIONS = -control
LPC21ISP_OPTIONS = -ADARM
# ---------------------------------------------------------------------------


# Define directories, if needed.
## DIRARM = c:/WinARM/
## DIRARMBIN = $(DIRAVR)/bin/
## DIRAVRUTILS = $(DIRAVR)/utils/bin/

# Define programs and commands.
SHELL = sh
CC = arm-none-eabi-gcc
CPP = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size
NM = arm-none-eabi-nm
REMOVE = rm -f
COPY = cp

#CC = arm-elf-gcc
#CPP = arm-elf-g++
#OBJCOPY = arm-elf-objcopy
#OBJDUMP = arm-elf-objdump
#SIZE = arm-elf-size
#NM = arm-elf-nm

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = "-------- begin (mode: $(RUN_MODE)) --------"
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_FLASH = Creating load file for Flash:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_COMPILING_ARM = "Compiling C (ARM-only):"
MSG_COMPILINGCPP = Compiling C++:
MSG_COMPILINGCPP_ARM = "Compiling C++ (ARM-only):"
MSG_ASSEMBLING = Assembling:
MSG_ASSEMBLING_ARM = "Assembling (ARM-only):"
MSG_CLEANING = Cleaning project:
MSG_FORMATERROR = Can not handle output-format
MSG_LPC21_RESETREMINDER = You may have to bring the target in bootloader-mode now.

# Define all object files.
COBJ      = $(SRC:.c=.o) 
AOBJ      = $(ASRC:.S=.o)
COBJARM   = $(SRCARM:.c=.o)
AOBJARM   = $(ASRCARM:.S=.o)
CPPOBJ    = $(CPPSRC:.cpp=.o) 
CPPOBJARM = $(CPPSRCARM:.cpp=.o)

# Define all listing files.
LST = $(ASRC:.S=.lst) $(ASRCARM:.S=.lst) $(SRC:.c=.lst) $(SRCARM:.c=.lst)
LST += $(CPPSRC:.cpp=.lst) $(CPPSRCARM:.cpp=.lst)

# Compiler flags to generate dependency files.
### GENDEPFLAGS = -Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).d
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS  = -mcpu=$(MCU) $(THUMB_IW) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mcpu=$(MCU) $(THUMB_IW) -I. -x assembler-with-cpp $(ASFLAGS)


# Default target.
all: begin gccversion sizebefore build sizeafter finished end

ifeq ($(FORMAT),ihex)
build: elf hex lss sym
hex: $(TARGET).hex
else 
ifeq ($(FORMAT),binary)
build: elf bin lss sym
bin: $(TARGET).bin
else 
$(error "$(MSG_FORMATERROR) $(FORMAT)")
endif
endif

elf: $(TARGET).elf $(TARGET).buildinfo
lss: $(TARGET).lss 
sym: $(TARGET).sym

# Eye candy.
begin:
	@echo
	@echo $(MSG_BEGIN)

finished:
	@echo $(MSG_ERRORS_NONE)

end:
	@echo $(MSG_END)
	@echo


# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) -A $(TARGET).elf
sizebefore:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); echo; fi

sizeafter:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi


# Display compiler version information.
gccversion : 
	@$(CC) --version


# Program the device.
ifeq ($(FLASH_TOOL),ULINK)
# Program the device with Keil's ULINK (needs configured uVision-Workspace). 
program: $(TARGET).hex
	@echo
	@echo "Programming with ULINK"
	C:\Keil\uv3\Uv3.exe -f ulinkflash.Uv2 -oulinkflash.txt
else
# Program the device.  - lpc21isp will not work for SAM7
program: $(TARGET).hex
	@echo
	@echo $(MSG_LPC21_RESETREMINDER)
	$(LPC21ISP) $(LPC21ISP_OPTIONS) $(LPC21ISP_DEBUG) $(LPC21ISP_FLASHFILE) $(LPC21ISP_PORT) $(LPC21ISP_BAUD) $(LPC21ISP_XTAL)
endif


# Create final output files (.hex, .eep) from ELF output file.
# TODO: handle an .eeprom-section but should be redundant
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) $< $@
	ln -sf $@ last.hex
	
# Create final output file (.bin) from ELF output file.
%.bin: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) $< $@
	ln -sf $@ last.bin


# Create extended listing file from ELF output file.
# testing: option -C
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S -C $< > $@
	ln -sf $@ last.lss


# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@
	ln -sf $@ last.sym


# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ) $(CPPOBJ) $(CPPOBJARM)
%.elf:  $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ) $(CPPOBJ) $(CPPOBJARM)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(THUMB) $(ALL_CFLAGS) $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ) $(CPPOBJ) $(CPPOBJARM) --output $@ $(LDFLAGS)
	ln -sf $@ last.elf

#	$(CPP) $(THUMB) $(ALL_CFLAGS) $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ) $(CPPOBJ) $(CPPOBJARM) --output $@ $(LDFLAGS)

$(TARGET).buildinfo:
	date > $@
	$(CC) --version >> $@
	-git describe --long >> $@
	-git describe --long --all >> $@
	@echo CC=$(CC) >> $@
	@echo THUMB=$(THUMB) >> $@
	@echo ALL_CFLAGS=$(ALL_CFLAGS) >> $@
	@echo AOBJARM=$(AOBJARM) >> $@
	@echo AOBJ=$(AOBJ) >> $@
	@echo COBJARM=$(COBJARM) >> $@
	@echo COBJ=$(COBJ) >> $@
	@echo CPPOBJ=$(CPPOBJ) >> $@
	@echo CPPOBJARM=$(CPPOBJARM) >> $@
	@echo LDFLAGS=$(LDFLAGS) >> $@
	@echo "Writing build information to $@"
	ln -sf $@ last.buildinfo

# Compile: create object files from C source files. ARM/Thumb
# Use CFLAGS_OUR_SRC flags only for our source code (all except located in Libraries dir)
$(COBJ) : %.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(THUMB) $(ALL_CFLAGS) $(CONLYFLAGS) `echo "$<" | grep -q Libraries || echo "${CFLAGS_OUR_SRC}"` $< -o $@

# Compile: create object files from C source files. ARM-only
$(COBJARM) : %.o : %.c
	@echo
	@echo $(MSG_COMPILING_ARM) $<
	$(CC) -c $(ALL_CFLAGS) $(CONLYFLAGS) $< -o $@ 

# Compile: create object files from C++ source files. ARM/Thumb
$(CPPOBJ) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP) $<
	$(CPP) -c $(THUMB) $(ALL_CFLAGS) $(CPPFLAGS) $< -o $@ 

# Compile: create object files from C++ source files. ARM-only
$(CPPOBJARM) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP_ARM) $<
	$(CPP) -c $(ALL_CFLAGS) $(CPPFLAGS) $< -o $@ 


# Compile: create assembler files from C source files. ARM/Thumb
## does not work - TODO - hints welcome
##$(COBJ) : %.s : %.c
##	$(CC) $(THUMB) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files. ARM/Thumb
$(AOBJ) : %.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(THUMB) $(ALL_ASFLAGS) $< -o $@


# Assemble: create object files from assembler source files. ARM-only
$(AOBJARM) : %.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING_ARM) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@


# Target: clean project.
clean: begin clean_list finished end


clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).bin
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).elf $(TARGET).buildinfo
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).a90
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lnk
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(COBJ)
	$(REMOVE) $(CPPOBJ)
	$(REMOVE) $(AOBJ)
	$(REMOVE) $(COBJARM)
	$(REMOVE) $(CPPOBJARM)
	$(REMOVE) $(AOBJARM)
	$(REMOVE) $(LST)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRC:.c=.d)
	$(REMOVE) $(SRCARM:.c=.s)
	$(REMOVE) $(SRCARM:.c=.d)
	$(REMOVE) $(CPPSRC:.cpp=.s) 
	$(REMOVE) $(CPPSRC:.cpp=.d)
	$(REMOVE) $(CPPSRCARM:.cpp=.s) 
	$(REMOVE) $(CPPSRCARM:.cpp=.d)
	$(REMOVE) .dep/*


# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex bin lss sym clean clean_list program

