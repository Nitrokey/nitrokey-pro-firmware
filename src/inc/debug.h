#ifndef DEBUG_H
#define DEBUG_H
#include <stddef.h>

// Uncomment the following line to enable debugging.
// If debugging is enabled, the host must listen to
// interrupt transfers on endpoint 5.
// #define DBG

// Sends the buffer contents as a series of interrupt
// transfers. All transfers are guaranteed to arrive
// before execution continues.
void Debug(uint8_t *buffer, size_t length);

// Wrapper for Debug with the same prototype as printf.
// Sends only up to 512 bytes. The remaining bytes are
// ignored. See Debug for more details.
void Debugf(const char *format, ...);
#endif
