#ifndef HAL_STRESS_H
#define HAL_STRESS_H

#include <stdint.h>

#define __IO volatile

typedef struct {
    __IO uint32_t CR;      /* Control Register */
    __IO uint32_t SR;      /* Status Register */
    __IO uint32_t DR;      /* Data Register */
    uint32_t      RESERVED[10];
} USART_TypeDef;

#define USART1_BASE     0x40011000UL
#define USART1          ((USART_TypeDef *) USART1_BASE)

#define USART_CR1_UE    (1U << 0)
#define USART_CR1_RE    (1U << 2)
#define USART_CR1_TE    (1U << 3)

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define __HAL_USART_ENABLE(__HANDLE__)   SET_BIT((__HANDLE__)->CR, USART_CR1_UE)
#define __HAL_USART_DISABLE(__HANDLE__)  CLEAR_BIT((__HANDLE__)->CR, USART_CR1_UE)

void USART_Init(USART_TypeDef *husart, uint32_t baudrate);
void USART_Transmit(USART_TypeDef *husart, uint8_t data);
uint8_t USART_Receive(USART_TypeDef *husart);

#endif /* HAL_STRESS_H */