CMD=bash -c "cd /app && make clean && make"
CMDCLEAN=bash -c "cd /app && make clean"

all: build

.PHONY: dfu
dfu:
	cd build/gcc && bash dfu.sh

.PHONY: build
build:
	echo `git describe` > build/gcc/GIT_VERSION
	sudo docker run -i --rm  -v ${PWD}:/app registry.git.dotplex.com/nitrokey/nitrokey-pro-firmware:compilation $(CMD)

.PHONY: clean
clean:
	sudo docker run -i --rm  -v ${PWD}:/app registry.git.dotplex.com/nitrokey/nitrokey-pro-firmware:compilation $(CMDCLEAN)
