// test_data/StressTestApp/stress_rtos_queue.c
// Focused on common RTOS Queue and Semaphore patterns used in real firmware.

#include <stdint.h>
#include <stdbool.h>

// === Simulated RTOS types ===
typedef void* QueueHandle_t;
typedef void* SemaphoreHandle_t;

typedef struct {
    uint32_t id;
    uint32_t value;
    uint32_t timestamp;
} SensorMessage_t;

// === Global shared objects (typical in RTOS apps) ===
static QueueHandle_t     sensor_queue = NULL;
static SemaphoreHandle_t data_mutex   = NULL;
static SemaphoreHandle_t binary_sem   = NULL;

// === Simulated RTOS API (for parser testing) ===
QueueHandle_t xQueueCreate(uint32_t length, uint32_t item_size);
bool xQueueSend(QueueHandle_t queue, const void* item, uint32_t timeout);
bool xQueueReceive(QueueHandle_t queue, void* buffer, uint32_t timeout);
bool xQueueSendFromISR(QueueHandle_t queue, const void* item, bool* higher_priority_woken);

SemaphoreHandle_t xSemaphoreCreateMutex(void);
SemaphoreHandle_t xSemaphoreCreateBinary(void);
bool xSemaphoreTake(SemaphoreHandle_t sem, uint32_t timeout);
bool xSemaphoreGive(SemaphoreHandle_t sem);
bool xSemaphoreGiveFromISR(SemaphoreHandle_t sem, bool* higher_priority_woken);

// === Producer Task ===
void SensorTask(void* arg) {
    SensorMessage_t msg = {0};

    sensor_queue = xQueueCreate(10, sizeof(SensorMessage_t));
    data_mutex   = xSemaphoreCreateMutex();
    binary_sem   = xSemaphoreCreateBinary();

    while (true) {
        // Read sensor (simulated)
        msg.id = 0x01;
        msg.value = 0xABCD;
        msg.timestamp = 12345;

        if (xSemaphoreTake(data_mutex, 100)) {
            if (xQueueSend(sensor_queue, &msg, 50)) {
                xSemaphoreGive(binary_sem);
            }
            xSemaphoreGive(data_mutex);
        }
    }
}

// === Consumer Task ===
void ProcessingTask(void* arg) {
    SensorMessage_t received;

    while (true) {
        if (xSemaphoreTake(binary_sem, 200)) {
            if (xSemaphoreTake(data_mutex, 50)) {
                if (xQueueReceive(sensor_queue, &received, 10)) {
                    // Process data
                    (void)received;
                }
                xSemaphoreGive(data_mutex);
            }
        }
    }
}

// === ISR that posts to queue ===
void DMA_Complete_IRQHandler(void) {
    SensorMessage_t msg = {
        .id = 0x02,
        .value = 0xDEAD,
        .timestamp = 99999
    };

    bool higher_priority_woken = false;

    if (sensor_queue != NULL) {
        xQueueSendFromISR(sensor_queue, &msg, &higher_priority_woken);
        xSemaphoreGiveFromISR(binary_sem, &higher_priority_woken);
    }
}