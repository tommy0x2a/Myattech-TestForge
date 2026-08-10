// test_data/StressTestApp/callback_table.c
// Demonstrates arrays of function pointers (callback tables)
// Very common in embedded drivers, state machines, and ISRs.

#include <stdint.h>
#include <stdbool.h>

typedef void (*EventHandler_t)(uint32_t event_data);

typedef struct {
    uint32_t        event_id;
    EventHandler_t  handler;
    bool            enabled;
} EventEntry_t;

// Callback table (array of function pointers)
static EventEntry_t event_table[] = {
    { .event_id = 0x01, .handler = NULL, .enabled = false },
    { .event_id = 0x02, .handler = NULL, .enabled = false },
    { .event_id = 0x03, .handler = NULL, .enabled = false },
};

#define EVENT_TABLE_SIZE (sizeof(event_table) / sizeof(event_table[0]))

void register_event_handler(uint32_t event_id, EventHandler_t handler) {
    for (uint32_t i = 0; i < EVENT_TABLE_SIZE; i++) {
        if (event_table[i].event_id == event_id) {
            event_table[i].handler = handler;
            event_table[i].enabled = true;
            return;
        }
    }
}

void dispatch_event(uint32_t event_id, uint32_t data) {
    for (uint32_t i = 0; i < EVENT_TABLE_SIZE; i++) {
        if (event_table[i].event_id == event_id && event_table[i].enabled) {
            if (event_table[i].handler != NULL) {
                event_table[i].handler(data);
            }
            return;
        }
    }
}

// Example handler
static void temperature_alert_handler(uint32_t data) {
    (void)data;
    // Would normally trigger some action
}