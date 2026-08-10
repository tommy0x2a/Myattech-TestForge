#ifndef MODULE_A_H
#define MODULE_A_H

#include <stdint.h>

/* Pure-logic module — no HAL dependency. */

uint8_t compute_checksum(const uint8_t *buf, uint16_t len);
int16_t clamp_value(int16_t value, int16_t lo, int16_t hi);

#endif /* MODULE_A_H */
