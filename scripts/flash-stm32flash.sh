FIRMWAREBIN=$1

if [ "$1xxx" == "xxx" ]; then
	echo "Firmware update script using stm32flash"
	echo "Please provide .hex file with firmware binary"
	exit 1
fi

echo "Running procedure with $FIRMWAREBIN"

stm32flash  -k /dev/ttyUSB0
stm32flash  -u /dev/ttyUSB0
stm32flash -v -w $FIRMWAREBIN /dev/ttyUSB0
stm32flash  -j /dev/ttyUSB0
