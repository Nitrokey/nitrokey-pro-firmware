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
        print("c",end='', flush=True)
        time.sleep(0.2)
        continue


    c = 1
    for config in dev:
        print (f"config {c}")
        print ('Interfaces {config.bNumInterfaces}')
        for i in range(config.bNumInterfaces):
            if dev.is_kernel_driver_active(i):
                dev.detach_kernel_driver(i)
            print (i)
        c+=1

    dev.set_configuration()
    print("DEVICE ATTACHED, LOGGING:")
    while True:
        try:
            b = dev.read(0x85, 32)
            sys.stdout.write(str(bytes(b)))
            sys.stdout.flush()
        except usb.core.USBError as e:
            if "Operation timed out" in str(e) or "Resource busy" in str(e):
                time.sleep(0.1)
                continue
            # print(f"COMMUNICATION ERROR: {e}")
            print('e',end='', flush=True)
