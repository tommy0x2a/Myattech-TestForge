/**
 * Minimal NXP Vendor HAL Emulation
 */
#ifndef HAL_GPIO_NXP_H
#define HAL_GPIO_NXP_H

#include <stdint.h>

/* Using exact same type definitions to challenge TestForge's duplicate namespace handling */
typedef struct { 
    uint32_t reserved; 
} GPIO_TypeDef;

void HAL_Init(void);
void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t PinState);

#endif /* HAL_GPIO_NXP_H */