ROOT_DIR=$(CURDIR)
BUILD_DIR=$(ROOT_DIR)/build/gcc
SCRIPT_DIR=$(ROOT_DIR)/scripts
OPENOCD_BIN?=

# Set version to anything required; real version will be provided in the .buildinfo anyway
VERSION=$(shell git describe)
PROJECT=nitrokey-hsm-firmware
ARCHIVE_NAME=$(PROJECT)-$(VERSION).tar.gz

# Set to "1" to run GPG signing on the hash files with the default key
SIGN=0

DEPS=gcc-arm-none-eabi

.PHONY: firmware flash-versaloon clean release

firmware:
	cd $(BUILD_DIR) && $(MAKE)

clean:
	cd $(BUILD_DIR) && \
	make clean
	-rm nitrokey-*-firmware*.tar.gz

deps:
	sudo apt-get install ${DEPS}

.PHONY: release release-all
release: | clean
	mkdir -p release
	-rm -r release/*.*
	$(MAKE) firmware -j12
	cp $(BUILD_DIR)/*.hex  release
	cp $(BUILD_DIR)/*.buildinfo  release
	#cd $(BUILD_DIR) && $(MAKE) -f dfu.mk firmware.hex VERSION=$(VERSION)
	#cd $(BUILD_DIR) && $(MAKE) -f dfu.mk all.hex VERSION=$(VERSION)
	#cp `readlink -f $(BUILD_DIR)/last_to_flash.hex` `readlink -f $(BUILD_DIR)/last.buildinfo` release/
	cd release && find . -name "*.hex" -type f -printf "%f\0" | xargs -0 -n1 -I{} sh -c 'sha512sum -b {} > {}.sha512'
	-[ $(SIGN) == 1 ] && cd release && ls -1 *.sha* | xargs -n1 gpg2 --detach-sign
	-[ $(SIGN) == 1 ] && cd release && ls -1 *.sig | xargs -n1 gpg2 --verify
	ls -lh release
	cd release && rm last*
	tar -czvf $(ARCHIVE_NAME) -C release . | sort

release-all:
	mkdir -p release-all
	-rm -r release-all/*.*
	$(MAKE) release
	mv *tar.gz release-all
	ls -lh release-all/

.PHONY: gdbserver
gdbserver:
	openocd -f interface/stlink-v2.cfg  -f target/stm32f1x.cfg

.PHONY: ocdtelnet
ocdtelnet:
	telnet localhost 4444

.PHONY: devloop
devloop: | clean
	$(MAKE) firmware -j12 BUILD_DEBUG=1
	-# killall telnet
	- killall openocd
	cd build/gcc && $(MAKE) -f dfu.mk flash-full-single
	$(MAKE) gdbserver > /dev/null &
	sleep 1
	$(MAKE) ocdtelnet

.PHONY: devloop-release
devloop-release: | clean
	$(MAKE) firmware -j12
	- killall openocd
	cd build/gcc && $(MAKE) -f dfu.mk flash-full-single