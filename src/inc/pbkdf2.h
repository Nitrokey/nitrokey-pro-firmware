#ifndef PBKDF2_H_
#define PBKDF2_H_

#include <stdint.h>

void pbkdf2(uint8_t *out, const uint8_t *password, const uint32_t password_length, const uint8_t *salt, const uint32_t salt_length);
uint32_t getPBKDF2Iterations (void);
void writeU32BigEndian (uint8_t * out, uint32_t in);

#endif // PBKDF2_H_