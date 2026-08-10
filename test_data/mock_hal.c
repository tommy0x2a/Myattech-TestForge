/**
 * TestForge — Auto-generated HAL mock implementation
 * Location: out/mock_hal.c
 * Generated: 2026-06-07T03:52:45.199315+00:00
 * WARNING: Auto-generated stub bodies. Replace with real mock logic.
 *          Or link against CMock-generated implementations.
 */

#include "mock_hal.h"

void HAL_Init(void) {
    /* stub — add tracking logic here if needed */
}

void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,
                       uint8_t PinState) {
    (void)GPIOx;
    (void)GPIO_Pin;
    (void)PinState;
}

void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    (void)GPIOx;
    (void)GPIO_Pin;
}