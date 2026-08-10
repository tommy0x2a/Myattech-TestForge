/**
 * TestForge — Auto-generated HAL mock header
 * Location: out/mock_hal.h
 * Generated: 2026-06-07T03:52:45.198667+00:00
 * WARNING: Auto-generated. Extend with real mock implementations
 *          or link against a mock framework like CMock.
 */

#ifndef MOCK_HAL_H
#define MOCK_HAL_H

#include <stdint.h>

/* HAL stub type definitions — GPIO_TypeDef defined exactly once */
/* Application types (GPIO_InitTypeDef etc.) are defined in the
   source file's own header — include that header before mock_hal.h */
typedef struct { uint32_t reserved; } GPIO_TypeDef;

/* HAL stub function declarations */
void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t PinState);
void HAL_Init(void);

#endif /* MOCK_HAL_H */
