#include <stdint.h>
#include <stdbool.h>

typedef void (*ErrorCallback_t)(uint32_t error_code);

typedef struct {
    uint32_t          timeout_ms;
    ErrorCallback_t   error_cb;
    volatile uint32_t *status_reg;
} PeripheralConfig_t;

bool Peripheral_Transfer(PeripheralConfig_t *cfg, uint8_t *buffer, uint32_t length) {
    if (cfg == NULL || buffer == NULL || length == 0) {
        if (cfg && cfg->error_cb) {
            cfg->error_cb(0x01); /* NULL parameter error */
        }
        return false;
    }

    if (cfg->timeout_ms == 0) {
        return false;
    }

    /* Complex guard with volatile register */
    if ((*cfg->status_reg & 0x01) == 0) {
        if (cfg->error_cb) {
            cfg->error_cb(0x02); /* Busy */
        }
        return false;
    }

    /* Simulate transfer */
    for (uint32_t i = 0; i < length; i++) {
        if ((*cfg->status_reg & 0x04) == 0) {
            break;
        }
        buffer[i] = 0xAA;
    }

    return true;
}