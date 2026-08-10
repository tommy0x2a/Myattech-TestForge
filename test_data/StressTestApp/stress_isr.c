// test_data/StressTestApp/stress_isr.c
// Focused on common and difficult Interrupt Service Routine (ISR) patterns.

#include <stdint.h>
#include <stdbool.h>

// === Shared data between ISR and main context ===
static volatile uint32_t g_tick_count = 0;
static volatile bool     g_data_ready = false;
static volatile uint32_t g_last_value = 0;

// === RTOS-style queue simulation (common in ISRs) ===
typedef struct {
    uint32_t id;
    uint32_t value;
} Message_t;

static Message_t g_isr_queue[4];
static volatile uint8_t g_queue_head = 0;
static volatile uint8_t g_queue_tail = 0;

// === UART RX ISR pattern ===
void UART_RX_IRQHandler(void) {
    // Read data register (volatile access)
    uint8_t data = *(volatile uint8_t*)0x40011004;

    g_last_value = data;
    g_data_ready = true;

    // Post to queue (ISR-safe pattern)
    uint8_t next_head = (g_queue_head + 1) % 4;
    if (next_head != g_queue_tail) {
        g_isr_queue[g_queue_head].id = 0x01;      // UART event
        g_isr_queue[g_queue_head].value = data;
        g_queue_head = next_head;
    }
}

// === SysTick / Timer ISR ===
void SysTick_Handler(void) {
    g_tick_count++;

    // Periodic task signaling
    if ((g_tick_count % 100) == 0) {
        g_data_ready = true;
    }
}

// === GPIO EXTI ISR with debouncing simulation ===
void EXTI15_10_IRQHandler(void) {
    // Clear interrupt flag (volatile register access)
    *(volatile uint32_t*)0x40010414 = (1U << 13);

    // Simple debouncing using tick count
    static uint32_t last_press_tick = 0;

    if ((g_tick_count - last_press_tick) > 5) {
        last_press_tick = g_tick_count;
        g_last_value = 0xDEAD;
        g_data_ready = true;
    }
}

// === Function that might be called from ISR (restricted) ===
bool isr_safe_queue_receive(Message_t* msg) {
    if (g_queue_head == g_queue_tail) {
        return false;
    }

    *msg = g_isr_queue[g_queue_tail];
    g_queue_tail = (g_queue_tail + 1) % 4;
    return true;
}