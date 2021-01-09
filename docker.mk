CMD=bash -c "cd /app && make clean && make"

all: build

.PHONY: dfu
dfu:
	cd build/gcc && bash dfu.sh

.PHONY: build
build:
	echo `git describe` > build/gcc/GIT_VERSION
	sudo docker run -i --rm  -v ${PWD}:/app registry.git.dotplex.com/nitrokey/nitrokey-pro-firmware:compilation $(CMD)
