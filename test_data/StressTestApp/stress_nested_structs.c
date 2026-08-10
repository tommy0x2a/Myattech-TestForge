// test_data/StressTestApp/stress_nested_structs.c

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t  priority;
    uint16_t timeout;
} TaskConfig_t;

typedef struct {
    TaskConfig_t config;
    struct {
        uint32_t stack_size;
        uint8_t  core_affinity;
        struct {
            bool     running;
            uint32_t runtime_ticks;
        } state;
    } internal;
} TaskControlBlock_t;

typedef struct {
    TaskControlBlock_t tasks[8];
    uint8_t            task_count;
} Scheduler_t;

void scheduler_add_task(Scheduler_t* sched, TaskControlBlock_t* tcb) {
    if (sched == NULL || tcb == NULL) return;

    if (sched->task_count < 8) {
        sched->tasks[sched->task_count] = *tcb;

        // Deeply nested access
        if (tcb->internal.state.running) {
            tcb->internal.state.runtime_ticks = 0;
        }

        sched->task_count++;
    }
}

bool is_task_ready(const TaskControlBlock_t* tcb) {
    if (tcb == NULL) return false;
    return tcb->internal.state.running && tcb->config.timeout > 0;
}