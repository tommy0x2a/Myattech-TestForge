#ifndef MODULE_B_H
#define MODULE_B_H

#include <stdint.h>

/* GPIO_TypeDef is provided by the vendor HAL (production builds) or by
 * TestForge's mock_hal.h (test builds).  The caller must include the
 * appropriate header before this one — the same convention as blink.h. */
void led_set_pin(GPIO_TypeDef *port, uint16_t pin, int state);

#endif /* MODULE_B_H */
