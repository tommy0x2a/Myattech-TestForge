// test_data/StressTestApp/stress_edge_cases.c

#include <stdint.h>
#include <stdbool.h>

typedef union {
    uint32_t word;
    struct {
        uint32_t enable     : 1;
        uint32_t mode       : 2;
        uint32_t reserved   : 5;
        uint32_t data       : 8;
        uint32_t            : 16;
    } bits;
} ControlRegister_t;

typedef struct {
    volatile ControlRegister_t CTRL;
    volatile uint32_t STATUS;
} Peripheral_t;

#define PERIPHERAL ((Peripheral_t*)0x40020000UL)

// Deeply nested macro
#define SET_MODE(reg, m) \
    do { \
        (reg).bits.mode = (m); \
        if ((reg).bits.enable) { \
            (reg).bits.data = 0xFF; \
        } \
    } while(0)

// Function pointer with context
typedef void (*EventCallback_t)(void* context, uint32_t event);

typedef struct {
    EventCallback_t callback;
    void*           user_data;
} EventHandler_t;

void process_peripheral(Peripheral_t* periph, EventHandler_t* handler) {
    if (periph == NULL || handler == NULL) return;

    ControlRegister_t ctrl = periph->CTRL;

    // Bitfield access
    if (ctrl.bits.enable && ctrl.bits.mode == 0x02) {
        SET_MODE(ctrl, 0x01);
    }

    // Union + volatile access
    if (periph->STATUS & 0x01) {
        if (handler->callback) {
            handler->callback(handler->user_data, periph->STATUS);
        }
    }

    // Nested macro usage
    SET_MODE(periph->CTRL, 0x03);
}

// Volatile pointer to array
void dma_transfer(volatile uint32_t* buffer, uint32_t size) {
    if (buffer == NULL || size == 0) return;

    for (uint32_t i = 0; i < size; i++) {
        buffer[i] = 0xDEADBEEF;
    }
}

// Complex callback registration
void register_event(EventHandler_t* handler, EventCallback_t cb, void* ctx) {
    if (handler == NULL) return;
    handler->callback = cb;
    handler->user_data = ctx;
}