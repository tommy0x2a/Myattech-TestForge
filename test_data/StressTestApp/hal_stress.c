#include "hal_stress.h"
#include <stddef.h>

void USART_Init(USART_TypeDef *husart, uint32_t baudrate) {
    if (husart == NULL) {
        return;
    }

    /* Heavy macro usage + volatile register access */
    __HAL_USART_DISABLE(husart);

    husart->CR = 0;
    husart->SR = 0;

    /* Simulate baud rate calculation with macros */
    uint32_t temp = (husart->CR & 0xFFFF0000U) | (baudrate & 0xFFFFU);
    husart->CR = temp;

    __HAL_USART_ENABLE(husart);
}

void USART_Transmit(USART_TypeDef *husart, uint8_t data) {
    if (husart == NULL || !READ_BIT(husart->SR, (1U << 7))) {
        return;   /* Guard condition */
    }

    husart->DR = data;

    /* Wait for transmission complete (volatile access) */
    while (!READ_BIT(husart->SR, (1U << 6))) {
        /* busy wait */
    }
}

uint8_t USART_Receive(USART_TypeDef *husart) {
    if (husart == NULL) {
        return 0;
    }

    while (!READ_BIT(husart->SR, (1U << 5))) {
        /* wait for data */
    }

    return (uint8_t)husart->DR;
}