/**
 * mock_hal.c — Phase 1 mock stubs for pico_i2c_driver and i2c_transport.
 *
 * All definitions are __attribute__((weak)) so the real implementations in
 * pico_i2c_driver_src.c override them in the pico_i2c_driver_tests binary.
 * sensor_aggregator_tests uses these stubs for pico_i2c_* (no real driver
 * compiled into that binary).
 *
 * After 'testforge generate', copy this file to out/test_data/demo/host/:
 *   cp test_data/demo/host/mock_hal.c out/test_data/demo/host/
 *
 * © 2026 Myattech LLC. All rights reserved.
 */

#include "i2c_transport.h"
#include "pico_i2c_driver.h"

/* ── i2c_transport stubs (used by pico_i2c_driver_tests) ─────────────── */

/* Static sentinel — lets pico_i2c_open() return a real heap handle without
   needing hardware.  Any non-NULL value satisfies the "open succeeded" check. */
static char _mock_transport_storage[64];

__attribute__((weak))
I2CTransport* i2c_transport_open(void)               { return (I2CTransport*)_mock_transport_storage; }

__attribute__((weak))
void i2c_transport_close(I2CTransport* t)             { (void)t; }

__attribute__((weak))
int i2c_transport_start(I2CTransport* t)              { (void)t; return 0; }

__attribute__((weak))
int i2c_transport_stop(I2CTransport* t)               { (void)t; return 0; }

__attribute__((weak))
int i2c_transport_write_byte(I2CTransport* t,
                              uint8_t byte)            { (void)t; (void)byte; return 0; }

__attribute__((weak))
uint8_t i2c_transport_read_byte(I2CTransport* t,
                                 int ack)              { (void)t; (void)ack; return 0; }

__attribute__((weak))
int i2c_transport_timed_out(I2CTransport* t)          { (void)t; return 0; }

/* ── pico_i2c_driver stubs (used by SensorAggregator_tests) ──────────── */
/* In pico_i2c_driver_tests these are overridden by pico_i2c_driver_src.c. */

/* Static backing storage for the mock handle.  PicoI2CHandle is opaque in the
   header, so we cannot use sizeof(PicoI2CHandle) here.  512 bytes is generous
   enough for any real struct body; it is zero-initialised at program start. */
static char _mock_handle_storage[512];

__attribute__((weak))
PicoI2CHandle* pico_i2c_open(void) {
    /* Return a pointer to zeroed static storage — never freed. */
    return (PicoI2CHandle*)_mock_handle_storage;
}

__attribute__((weak))
void pico_i2c_close(PicoI2CHandle* handle)            { (void)handle; }

__attribute__((weak))
PicoI2CStatus pico_i2c_read_register(PicoI2CHandle* handle,
                                      uint8_t        reg,
                                      uint8_t*       out) {
    (void)handle; (void)reg; (void)out;
    return I2C_OK;
}

__attribute__((weak))
PicoI2CStatus pico_i2c_write_register(PicoI2CHandle* handle,
                                       uint8_t        reg,
                                       uint8_t        value) {
    (void)handle; (void)reg; (void)value;
    return I2C_OK;
}

__attribute__((weak))
PicoI2CStatus pico_i2c_ping(PicoI2CHandle* handle)   { (void)handle; return I2C_OK; }
