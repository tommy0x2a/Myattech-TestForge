/**
 * Minimal STM32 Vendor HAL Emulation
 */
#ifndef HAL_GPIO_STM32_H
#define HAL_GPIO_STM32_H

#include <stdint.h>

/* Minimal typedef matching your TestForge screenshot output */
typedef struct { 
    uint32_t reserved; 
} GPIO_TypeDef;

/* Target vendor functions for TestForge to discover and mock */
void HAL_Init(void);
void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t PinState);

#endif /* HAL_GPIO_STM32_H */