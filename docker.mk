DOCKER_BIN=docker

all:
	$(DOCKER)

.PHONY: build udev openocd openocd-server flash
build:
	$(DOCKER_BIN) build -t nkpro .
udev:
	wget https://raw.githubusercontent.com/stlink-org/stlink/eb35054ea646ef80b1d57cd3a26bc00583f38d08/config/udev/rules.d/49-stlinkv2.rules
	sudo mv -v 49-stlinkv2.rules /etc/udev/rules.d
	sudo udevadm control --reload-rules
	sudo udevadm trigger

# add --userns=keep-id for podman users
DOCKER=$(DOCKER_BIN) run -it --rm --privileged -v $(PWD):/app:Z -p 4444:4444 -p 3333:3333 --network=host  localhost/nkpro
OPENOCD=openocd -f interface/stlink.cfg -f target/stm32f1x.cfg
openocd:
	$(OPENOCD) 

openocd-server:
	$(DOCKER) $(OPENOCD) -c "program $(FLASH) verify reset"

FLASH=/app/build/gcc/nitrokey-hsm-firmware.elf
flash:
	$(DOCKER) $(OPENOCD) -c "program $(FLASH) verify reset exit"
