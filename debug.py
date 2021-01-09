import usb.core
import sys
import time

# This script waits for a Nitrokey HSM in debug mode to be attached
# and prints all debug output. Timeouts are ignored. All other 
# communication errors produce an error message and the script
# continues by waiting for a device to be attached.
# Make sure the script is running before the Nitrokey is attached
# to prevent timeouts in other applications.

while True:
    # TODO: Other device types
    dev = usb.core.find(idVendor=0x20a0, idProduct=0x4230)
    if dev is None:
        time.sleep(0.1)
        continue
    dev.set_configuration()
    print("DEVICE ATTACHED, LOGGING:")
    while True:
        try:
            bytes = dev.read(0x85, 32)
            sys.stdout.write(bytes.tostring())
            sys.stdout.flush()
        except usb.core.USBError as e:
            print("COMMUNICATION ERROR:")
            print(e)
            time.sleep(0.1)
            break
