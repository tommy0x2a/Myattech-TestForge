// test_data/StressTestApp/mega_stress.c
// Mega stress test file — combines many hard embedded patterns.

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define __IO volatile

// === 1. Heavy macros + bit manipulation ===
#define SET_BIT(reg, bit)    ((reg) |= (1U << (bit)))
#define CLEAR_BIT(reg, bit)  ((reg) &= ~(1U << (bit)))
#define READ_BIT(reg, bit)   ((reg) & (1U << (bit)))

// === 2. Union + Bitfields (hardware register style) ===
typedef union {
    uint32_t word;
    struct {
        uint32_t enable     : 1;
        uint32_t mode       : 2;
        uint32_t data_ready : 1;
        uint32_t            : 28;
    } bits;
} ControlReg_t;

// === 3. Deeply nested structs ===
typedef struct {
    uint32_t stack_size;
    struct {
        bool     active;
        uint32_t runtime;
        struct {
            uint8_t  priority;
            bool     preemptible;
        } sched;
    } state;
} TaskControlBlock_t;

// === 4. Callback table ===
typedef void (*Callback_t)(void* context);

typedef struct {
    uint32_t   id;
    Callback_t cb;
    void*      ctx;
} CallbackEntry_t;

static CallbackEntry_t callback_table[8];

// === 5. Volatile hardware register simulation ===
typedef struct {
    __IO ControlReg_t CTRL;
    __IO uint32_t     STATUS;
    __IO uint32_t     DATA;
} Peripheral_t;

#define PERIPH ((Peripheral_t*)0x40010000UL)

// === 6. Complex function with many guards ===
bool process_data(Peripheral_t* periph, TaskControlBlock_t* tcb, uint32_t value) {
    if (periph == NULL || tcb == NULL) {
        return false;
    }

    if (!periph->CTRL.bits.enable) {
        return false;
    }

    if (READ_BIT(periph->STATUS, 0)) {
        return false; // Busy
    }

    // Deeply nested access
    if (tcb->state.sched.priority > 5 && tcb->state.active) {
        periph->DATA = value;
        SET_BIT(periph->CTRL.word, 0);
        return true;
    }

    return false;
}

// === 7. Function pointer dispatch ===
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