/**
 * Sample Hardware Agnostic Blink Application Interface
 */
#ifndef BLINK_H
#define BLINK_H

#include <stdint.h>

/* Application-level struct matching your TestForge UI log */
typedef struct {
    uint16_t Pin;
    uint8_t  Mode;
    uint8_t  Speed;
} GPIO_InitTypeDef;

/* Function prototypes for TestForge to parse */
void SystemClock_Config(void);
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_Init);
void delay_ms(uint32_t ms);
int main(void);

#endif /* BLINK_H */