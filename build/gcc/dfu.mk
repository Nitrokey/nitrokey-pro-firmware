#!/usr/bin/env -S make 

# 1. To get STM32_Programmer_CLI see:
# - https://www.st.com/en/development-tools/stm32cubeprog.html (2021-06)
#	OpenOCD can be used instead.
# 2. srec_* tools are part of the srecord package (Fedora 32+; see setup-fedora recipe)
# 3. Bootloader activation is at the moment available only through libnitrokey test suite, soon to be through pynitrokey.
# 4. dfu-util should be available as a dfu-util package (Fedora 32+; see setup-fedora recipe)
# 5. arm-none-eabi-objcopy is a part of the GCC ARM suite

WORKSPACE=~/work
BOOTLOADER=$(WORKSPACE)/dapboot/build/dapboot-nkpro.bin
FIRMWARE=last.elf
FIRMWAREBIN=last.bin

.PHONY: flash-bootloader
flash-bootloader: $(BOOTLOADER)
	STM32_Programmer_CLI -c port=SWD -halt  --readunprotect
	STM32_Programmer_CLI -c port=swd -e all -w $< 0x8000000 -v -rst
	ls -lh $<

%.bin: %.elf
	arm-none-eabi-objcopy -O binary $< $@ 
	date
	ls -lh $<

.PHONY: flash-dfu
flash-dfu: $(FIRMWAREBIN)
	sudo dfu-util -D $<

.PHONY: flash-full
flash-full: | flash-bootloader flash-dfu

bootloader.hex: $(BOOTLOADER)
	srec_cat $< -Binary -offset 0x8000000 -Output $@ -Intel 
	ls -lh $@
	srec_info $@ -i

FIRMWARE_FILE_NAME_UPDATE=nitrokey-pro-firmware-$(shell git describe --long)-to_update.hex
firmware.hex: $(FIRMWAREBIN)
	srec_cat $< -Binary -offset 0x8002000 -Output $@ -Intel 
	cp $@ $(FIRMWARE_FILE_NAME_UPDATE)
	ln -sf $(FIRMWARE_FILE_NAME_UPDATE) last_update.hex
	ls -lh $@
	srec_info $@ -i

FIRMWARE_FILE_NAME=nitrokey-pro-firmware-$(shell git describe --long)-to_flash.hex
all.hex: bootloader.hex firmware.hex
	srec_cat bootloader.hex -Intel firmware.hex -Intel -Output $@ -Intel
	cp $@ $(FIRMWARE_FILE_NAME)
	ls -lh $@
	srec_info $@ -i

SINGLE_FW=all.hex
.PHONY: flash-full-single
flash-full-single: $(SINGLE_FW)
	STM32_Programmer_CLI -c port=SWD -halt  --readunprotect
	STM32_Programmer_CLI -c port=SWD -halt  --erase all
	# STM32_Programmer_CLI -c port=SWD -halt  -u 0x8000000 64000 clear-check.hex
	STM32_Programmer_CLI -c port=swd -e all -w $< 0x8000000 -v -rst

.PHONY: activate-bootloader
activate-bootloader:
	cd $(WORKSPACE)/libnitrokey/unittest && pytest test_pro_bootloader.py  -k test_bootloader_run_pro_real -svx --run-skipped

.PHONY: reset
reset:
	STM32_Programmer_CLI -c port=swd -rst

.PHONY: setup-fedora
setup-fedora:
	sudo dnf install srecord dfu-util