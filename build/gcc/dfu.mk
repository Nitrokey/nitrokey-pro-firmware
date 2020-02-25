#!/usr/bin/env -S make 

BOOTLOADER=../../../dapboot/build/dapboot-nkpro.bin
FIRMWARE=nitrokey-pro-firmware.elf 
FIRMWAREBIN=nitrokey-pro-firmware.bin 

.PHONY: flash-bootloader
flash-bootloader: $(BOOTLOADER)
	STM32_Programmer_CLI -c port=SWD -halt  --readunprotect
	STM32_Programmer_CLI -c port=swd -e all -w $< 0x8000000 -v -rst

%.bin: %.elf
	arm-none-eabi-objcopy -O binary $< $@ 

.PHONY: flash-dfu
flash-dfu: $(FIRMWAREBIN)
	sleep 1
	sudo dfu-util -D $<

.PHONY: flash-full
flash-full: | flash-bootloader flash-dfu

bootloader.hex: $(BOOTLOADER)
	srec_cat $< -Binary -offset 0x8000000 -Output $@ -Intel 
	ls -lh $@
	srec_info $@ -i

firmware.hex: $(FIRMWAREBIN)
	srec_cat $< -Binary -offset 0x8002000 -Output $@ -Intel 
	ls -lh $@
	srec_info $@ -i

all.hex: bootloader.hex firmware.hex
	srec_cat bootloader.hex -Intel firmware.hex -Intel -Output $@ -Intel
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
	cd ~/work/libnitrokey/unittest && pytest test_pro_bootloader.py  -k test_bootloader_run_pro_real -svx --run-skipped
