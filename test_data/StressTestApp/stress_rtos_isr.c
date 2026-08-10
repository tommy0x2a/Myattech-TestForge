// test_data/StressTestApp/stress_rtos_isr.c

#include <stdint.h>
#include <stdbool.h>

typedef void* QueueHandle_t;

typedef struct {
    uint32_t id;
    uint32_t value;
    uint32_t timestamp;
} Message_t;

extern QueueHandle_t xQueueCreate(uint32_t length, uint32_t item_size);
extern bool xQueueSendFromISR(QueueHandle_t q, const void* item, bool* higher_priority_woken);
extern bool xQueueReceive(QueueHandle_t q, void* buffer, uint32_t timeout);

static QueueHandle_t sensor_queue = NULL;

void vSensorISR(void) {
    Message_t msg = {
        .id = 0x01,
        .value = 0xDEAD,
        .timestamp = 12345
    };

    bool higher_priority_woken = false;

    if (sensor_queue != NULL) {
        xQueueSendFromISR(sensor_queue, &msg, &higher_priority_woken);
    }
}

void SensorTask(void* arg) {
    Message_t received;

    sensor_queue = xQueueCreate(10, sizeof(Message_t));

    while (true) {
        if (xQueueReceive(sensor_queue, &received, 100)) {
            // Process message
            if (received.id == 0x01) {
                // Handle sensor data
            }
        }
    }
}