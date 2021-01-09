#include <stdarg.h>
#include <stdio.h>
#include "usb_lib.h"
#include "CCIDHID_usb_conf.h"
#include "CCIDHID_usb_desc.h"
#include "usb_pwr.h"
#include "usb_bot.h"
#include "hw_config.h"
#include "CCIDHID_usb_prop.h"
#include "CCIDHID_usb.h"
#include "debug.h"

__IO uint8_t DebugTransferComplete;

void Debug(uint8_t *buffer, size_t length) {
#ifdef DBG
    if (!INITIALIZED) return;
    uint32_t cnt = 0xFF;
  // Split buffer into transfers of length 32 or less, which are
  // sent to the host individually.
  while (length != 0) {
    // Determine next transfer length (32 bytes or less)
    size_t messageLength = 32;
    if (length < messageLength)
      messageLength = length;
    // Start transfer
    DebugTransferComplete = 0;
    UserToPMABufferCopy(buffer, ENDP5_TXADDR, messageLength);
    SetEPTxCount(ENDP5, messageLength);
    SetEPTxStatus(ENDP5, EP_TX_VALID);
    // Wait for content to arrive at host
    while (DebugTransferComplete == 0 && cnt-->1)
      ;
    // Move forward in buffer
    length -= messageLength;
    buffer += messageLength;
  }
#endif
}

void Debugf(const char *format, ...) {
#ifdef DBG \
    if (!INITIALIZED) return;
  char buffer[512];
  va_list args;
  va_start(args, format);
  int length = vsnprintf(buffer, 512, format, args);
  va_end(args);
  if (length > 0)
    Debug((uint8_t *)buffer, (size_t)length);
#endif
}
