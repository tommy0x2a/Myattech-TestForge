// test_data/StressTestApp/mega_stress_with_asm.c
// Mega stress test with inline assembly, bitfields, callbacks, nested structs, etc.

#include <stdint.h>
#include <stdbool.h>

#define __IO volatile

// === Hardware register union + bitfields ===
typedef union {
    uint32_t word;
    struct {
        uint32_t enable     : 1;
        uint32_t mode       : 2;
        uint32_t data_ready : 1;
        uint32_t            : 28;
    } bits;
} ControlReg_t;

typedef struct {
    __IO ControlReg_t CTRL;
    __IO uint32_t     STATUS;
    __IO uint32_t     DATA;
} Peripheral_t;

#define PERIPH ((Peripheral_t*)0x40010000UL)

// === Deeply nested task structure ===
typedef struct {
    uint32_t stack_size;
    struct {
        bool active;
        uint32_t runtime;
        struct {
            uint8_t priority;
            bool preemptible;
        } sched;
    } state;
} TaskControlBlock_t;

// === Callback table ===
typedef void (*Callback_t)(void* ctx);

typedef struct {
    uint32_t   id;
    Callback_t cb;
    void*      ctx;
} CallbackEntry_t;

static CallbackEntry_t callback_table[8];

// === Inline assembly critical section (ARM Cortex-M) ===
static inline void enter_critical(void) {
    __asm volatile ("cpsid i" : : : "memory");
}

static inline void exit_critical(void) {
    __asm volatile ("cpsie i" : : : "memory");
}

// === Function with inline assembly + guards ===
bool atomic_set_bit(volatile uint32_t* reg, uint32_t bit) {
    uint32_t result;
    enter_critical();

    __asm volatile (
        "ldr r0, [%1]\n"
        "orr r0, r0, %2\n"
        "str r0, [%1]\n"
        "mov %0, #1\n"
        : "=r" (result)
        : "r" (reg), "r" (1U << bit)
        : "r0", "memory"
    );

    exit_critical();
    return result;
}

// === Complex processing function ===
bool process_with_asm(Peripheral_t* periph, TaskControlBlock_t* tcb, uint32_t value) {
    if (periph == NULL || tcb == NULL) return false;

    if (!periph->CTRL.bits.enable) return false;

    enter_critical();

    if (READ_BIT(periph->STATUS, 0)) {
        exit_critical();
        return false;
    }

    periph->DATA = value;

    // Use inline assembly to set bit atomically
    atomic_set_bit(&periph->CTRL.word, 0);

    // Deep nested struct access
    if (tcb->state.sched.priority > 5) {
        tcb->state.runtime++;
    }

    exit_critical();
    return true;
}

// === Callback registration and dispatch ===
void register_callback(uint32_t id, Callback_t cb, void* ctx) {
    for (int i = 0; i < 8; i++) {
        if (callback_table[i].id == 0 || callback_table[i].id == id) {
            callback_table[i].id = id;
            callback_table[i].cb = cb;
            callback_table[i].ctx = ctx;
            return;
        }
    }
}

void trigger_callback(uint32_t id) {
    for (int i = 0; i < 8; i++) {
        if (callback_table[i].id == id && callback_table[i].cb != NULL) {
            callback_table[i].cb(callback_table[i].ctx);
            return;
        }
    }
}