/**
 * Minimal NXP Vendor HAL Implementation
 */
#include "hal_gpio.h"

void HAL_Init(void) {
    // NXP Specific Clocking Gate Enable
}

void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    // Different underlying layout logic to verify AST depth parsing
    if (GPIOx) {
        GPIOx->reserved ^= (1 << GPIO_Pin);
    }
}

void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t PinState) {
    if (GPIOx) {
        // NXP boundary condition check: PinState can evaluate to 0 or 1
        if (PinState == 0) {
            GPIOx->reserved &= ~(1 << GPIO_Pin);
        } else {
            GPIOx->reserved |= (1 << GPIO_Pin);
        }
    }
}