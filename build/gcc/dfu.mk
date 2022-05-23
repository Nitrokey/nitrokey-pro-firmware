#!/usr/bin/env -S make 

WORKSPACE=~/work
DAPBOOT_DIR=../../dapboot
BOOTLOADER=$(DAPBOOT_DIR)/build/dapboot-nkpro.bin
FIRMWARE=last.elf
FIRMWAREBIN=last.bin

$(DAPBOOT_DIR)/libopencm3/Makefile:
	git submodule update --init --recursive

$(BOOTLOADER):
	cd $(DAPBOOT_DIR) && make -f release.Makefile clean && make -f release.Makefile

.PHONY: clean
clean:
	cd $(DAPBOOT_DIR) && make -f release.Makefile clean


.PHONY: flash-bootloader
flash-bootloader: $(BOOTLOADER)
	STM32_Programmer_CLI -c port=SWD -halt  --readunprotect
	STM32_Programmer_CLI -c port=swd -e all -w $< 0x8000000 -v -rst
	ls -lh $<

%.bin: %.elf
	arm-none-eabi-objcopy -O binary $< $@ 
	date
	ls -lh $<

NITROKEY_VID := 20a0
BOOTLOADER_PID := 42b4
OPERATION_PID := 4108

.PHONY: flash-dfu
flash-dfu: | $(FIRMWAREBIN) device_update
	sudo dfu-util -D $(FIRMWAREBIN)
	sleep 2

.PHONY: flash-full
flash-full: | flash-bootloader flash-dfu

bootloader.hex: $(BOOTLOADER)
	srec_cat $< -Binary -offset 0x8000000 -Output $@ -Intel 
	ls -lh $@
	srec_info $@ -i
	@echo $(realpath $@)

VERSION=$(shell git describe)
FIRMWARE_FILE_NAME_UPDATE=nitrokey-pro-firmware-$(VERSION)-to_update.bin
$(FIRMWARE_FILE_NAME_UPDATE): $(FIRMWAREBIN)
	cp $< $@
	ln -sf $(FIRMWARE_FILE_NAME_UPDATE) last_update.bin

firmware.hex: $(FIRMWAREBIN)
	srec_cat $< -Binary -offset 0x8002000 -Output $@ -Intel 
	ls -lh $@
	srec_info $@ -i

FIRMWARE_FILE_NAME=nitrokey-pro-firmware-$(VERSION)-to_flash.hex
all.hex: bootloader.hex firmware.hex $(FIRMWARE_FILE_NAME_UPDATE)
	srec_cat bootloader.hex -Intel firmware.hex -Intel -Output $@ -Intel
	cp $@ $(FIRMWARE_FILE_NAME)
	ln -sf $(FIRMWARE_FILE_NAME) last_to_flash.hex
	ls -lh $@
	srec_info $@ -i

SINGLE_FW=all.hex
.PHONY: flash-full-single
flash-full-single: $(SINGLE_FW)
	STM32_Programmer_CLI -c port=SWD -halt  --readunprotect
	STM32_Programmer_CLI -c port=SWD -halt  --erase all
	# STM32_Programmer_CLI -c port=SWD -halt  -u 0x8000000 64000 clear-check.hex
	STM32_Programmer_CLI -c port=swd -e all -w $< 0x8000000 -v -rst

DOWNLOAD=download.hex
PHONY: download-image
download-image:
	STM32_Programmer_CLI -c port=SWD -halt  -u 0x8000000 $$((128*1024)) $(DOWNLOAD)
	ls -lh $(DOWNLOAD)

.PHONY: update
update: | activate-bootloader flash-dfu run_quick_test

.PHONY: device_operation
device_operation:
	lsusb | grep ${NITROKEY_VID}:${OPERATION_PID} || sleep 1
	lsusb | grep ${NITROKEY_VID}:${OPERATION_PID} || sleep 1
	lsusb | grep ${NITROKEY_VID}:${OPERATION_PID} || sleep 1

.PHONY: device_update
device_update:
	lsusb | grep ${NITROKEY_VID}:${BOOTLOADER_PID} || sleep 1
	lsusb | grep ${NITROKEY_VID}:${BOOTLOADER_PID} || sleep 1
	lsusb | grep ${NITROKEY_VID}:${BOOTLOADER_PID} || sleep 1

.PHONY: run_quick_test
TESTS=get_status
PYTEST_ARG=test_pro.py -k $(TESTS) -vx
run_quick_test: | device_operation
	cd $(WORKSPACE)/libnitrokey/unittest && pipenv run pytest $(PYTEST_ARG)

.PHONY: activate-bootloader
activate-bootloader: device_operation
	cd $(WORKSPACE)/libnitrokey/unittest && pipenv run pytest test_pro_bootloader.py  -k test_bootloader_run_pro_real -svx --run-skipped

.PHONY: reset
reset:
	STM32_Programmer_CLI -c port=swd -rst
