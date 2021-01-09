from datetime import date, datetime
from logging import getLogger

import usb.core
import sys
import time

# This script waits for a Nitrokey HSM in debug mode to be attached
# and prints all debug output. Timeouts are ignored. All other 
# communication errors produce an error message and the script
# continues by waiting for a device to be attached.
# Make sure the script is running before the Nitrokey is attached
# to prevent timeouts in other applications.

log = getLogger('debug')

def detach_kernel():
    c = 1
    for config in dev:
        print(f"config {c}")
        print('Interfaces {config.bNumInterfaces}')
        for i in range(config.bNumInterfaces):
            if dev.is_kernel_driver_active(i):
                dev.detach_kernel_driver(i)
            print(i)
        c += 1


log_count = 1

def log_received_data(b):
    global log_count
    log_count += 1
    if b'<< USART wait' not in bytes(b):
        sys.stdout.write("\n")
        sys.stdout.write(str(datetime.now()))
        sys.stdout.write(f"\t{log_count}\t")
        sys.stdout.write(str(bytes(b)))
        sys.stdout.write("\n")
    else:
        sys.stdout.write('ruw ')
    sys.stdout.flush()


while True:
    # TODO: Other device types
    dev = usb.core.find(idVendor=0x20a0, idProduct=0x4230)
    if dev is None:
        print("c",end='', flush=True)
        time.sleep(0.2)
        continue

    detach_kernel()

    dev.set_configuration()
    print("DEVICE ATTACHED, LOGGING:")
    while True:
        try:
            b = dev.read(0x85, 32)
            log_received_data(b)
        except usb.core.USBError as e:
            if "Operation timed out" in str(e) or "Resource busy" in str(e):
                continue
            # print(f"COMMUNICATION ERROR: {e}")
            print('e',end='', flush=True)
            break
        except KeyboardInterrupt as e:
            break
        except Exception as e:
            print(e)
            break
