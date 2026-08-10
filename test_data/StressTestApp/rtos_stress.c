#include <stdint.h>
#include <stdbool.h>

typedef void* TaskHandle_t;
typedef void* SemaphoreHandle_t;
typedef void* QueueHandle_t;

typedef struct {
    volatile uint32_t tick_count;
    volatile bool     system_ready;
    uint32_t          stack_size;
} SystemState_t;

extern SystemState_t g_system_state;

void vTaskCreate(TaskHandle_t *task, void (*func)(void*), void *arg, uint32_t priority);
void vTaskDelay(uint32_t ticks);
bool xSemaphoreTake(SemaphoreHandle_t sem, uint32_t timeout);
void xSemaphoreGive(SemaphoreHandle_t sem);

void SensorTask(void *arg) {
    SemaphoreHandle_t data_ready = (SemaphoreHandle_t)arg;

    while (g_system_state.system_ready) {
        if (xSemaphoreTake(data_ready, 100)) {
            /* Critical section with volatile access */
            g_system_state.tick_count++;

            if (g_system_state.tick_count > 10000) {
                g_system_state.system_ready = false;
            }

            xSemaphoreGive(data_ready);
        }
        vTaskDelay(10);
    }
}

void SystemInit(void) {
    g_system_state.tick_count = 0;
    g_system_state.system_ready = true;
}