#include "module_b.h"

/* HAL_GPIO_WritePin is declared by the test build's mock_hal.h (included
 * in the source wrapper before this file) or by the vendor stm32f4xx_hal_gpio.h
 * in a production build.  This source file does not include HAL headers
 * directly so the test toolchain can substitute mock declarations cleanly. */
void led_set_pin(GPIO_TypeDef *port, uint16_t pin, int state)
{
    HAL_GPIO_WritePin(port, pin, (uint8_t)state);
}
