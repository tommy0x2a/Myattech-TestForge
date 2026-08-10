/**
 * Sample Hardware Agnostic Blink Application Implementation
 */
#include "blink.h"
#include "hal/stm32/hal_gpio.h" // In a real project, these are conditionally compiled
#include "hal/nxp/hal_gpio.h"   // or abstracted behind a target macro

void SystemClock_Config(void) {
    // TestForge should see this call and expect HAL_Init() in mocks
    HAL_Init(); 
}

void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_Init) {
    /* TestForge parsing goal: Boundary checks for NULL pointers 
       and verification of internal configuration logic */
    if (GPIOx == NULL || GPIO_Init == NULL) {
        return; 
    }
    
    // Simulate setting up a pin using a vendor HAL call
    HAL_GPIO_WritePin(GPIOx, GPIO_Init->Pin, 0);
}

void delay_ms(uint32_t ms) {
    /* TestForge parsing goal: Read the uint32_t data type and 
       extract limits (0, 1, 4294967295) for edge case testing */
    for (volatile uint32_t i = 0; i < ms; i++) {
        // Fake delay loop constraint
        if (ms > 5000) {
            // TestForge boundary target: 5000ms limit condition block
            break;
        }
    }
}

int main(void) {
    SystemClock_Config();
    
    GPIO_InitTypeDef led_config = { .Pin = 12, .Mode = 1, .Speed = 3 };
    GPIO_TypeDef fake_port;
    
    GPIO_Init(&fake_port, &led_config);
    
    while (1) {
        // Toggle sequence to exercise vendor code
        HAL_GPIO_TogglePin(&fake_port, led_config.Pin);
        delay_ms(500);
    }
    return 0;
}