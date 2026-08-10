#include "module_a.h"

uint8_t compute_checksum(const uint8_t *buf, uint16_t len)
{
    uint8_t sum = 0;
    uint16_t i;
    if (!buf) return 0;
    for (i = 0; i < len; i++) {
        sum += buf[i];
    }
    return sum;
}

int16_t clamp_value(int16_t value, int16_t lo, int16_t hi)
{
    if (value < lo) return lo;
    if (value > hi) return hi;
    return value;
}
