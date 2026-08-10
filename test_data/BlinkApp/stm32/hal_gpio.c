/**
 * Minimal STM32 Vendor HAL Implementation
 */
#include "hal_gpio.h"

void HAL_Init(void) {
    /* STM32 Specific Clocking Gate Enable */
}

void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    if (GPIOx) {
        GPIOx->reserved ^= (1 << GPIO_Pin);
    }
}

void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t PinState) {
    if (GPIOx) {
        if (PinState == 0) {
            GPIOx->reserved &= ~(1 << GPIO_Pin);
        } else {
            GPIOx->reserved |= (1 << GPIO_Pin);
        }
    }
}