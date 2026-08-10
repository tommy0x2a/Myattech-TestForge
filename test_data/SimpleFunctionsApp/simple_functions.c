#include <stdint.h>

void no_params(void) {
    // empty
}

uint32_t two_params(uint32_t a, uint32_t b) {
    return a + b;
}

static void static_func(void) {}

inline void inline_func(void) {}

void pointer_param(const char* str) {}

void array_param(int arr[10]) {}