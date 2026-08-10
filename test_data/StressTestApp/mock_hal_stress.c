// test_data/StressTestApp/mock_hal_stress.c
// Mock implementations for HAL functions used in stress tests.
// These allow generated tests to compile and link without real hardware.

#include <stdint.h>
#include <stdbool.h>
#include "hal_stress.h"

// Mock USART peripheral state
static USART_TypeDef mock_usart1 = {0};

USART_TypeDef* USART1 = &mock_usart1;

// Mock HAL functions
void HAL_Init(void) {
    // No-op for unit testing
}

void HAL_GPIO_WritePin(void* GPIOx, uint16_t Pin, uint8_t PinState) {
    (void)GPIOx;
    (void)Pin;
    (void)PinState;
    // Could add logging or state tracking here if needed
}

void HAL_GPIO_TogglePin(void* GPIOx, uint16_t Pin) {
    (void)GPIOx;
    (void)Pin;
}

void HAL_Delay(uint32_t Delay) {
    (void)Delay;
    // In unit tests we usually don't want real delays
}

// USART mock implementations (used by hal_stress.c)
void USART_Init(USART_TypeDef *husart, uint32_t baudrate) {
    if (husart == NULL) return;
    husart->CR = 0;
    husart->SR = 0x80; // TXE set
    (void)baudrate;
}

void USART_Transmit(USART_TypeDef *husart, uint8_t data) {
    if (husart == NULL) return;
    husart->DR = data;
    husart->SR |= 0x40; // TC set
}

uint8_t USART_Receive(USART_TypeDef *husart) {
    if (husart == NULL) return 0;
    return (uint8_t)husart->DR;
}