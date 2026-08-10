/**
 * pico_i2c_driver.h
 *
 * I2C driver for the Sensor Aggregator demo Pico 2 W.
 * Uses i2c_transport.h for hardware access (testable without libftdi).
 * TestForge mocks i2c_transport.h to test this driver in isolation.
 *
 * PicoI2CHandle is an opaque type: callers hold a pointer and never access
 * internals. pico_i2c_open() returns a heap-allocated handle; pass it to
 * pico_i2c_close() when done to release resources.
 *
 * CJMCU FT232H wiring:
 *   D0  → Pico GP5 (SCL)
 *   D1  → Pico GP4 (SDA)  ← tie D1+D2 together
 *   D2  → Pico GP4 (SDA)
 *   GND → Pico GND
 *
 * © 2026 Myattech LLC. All rights reserved.
 */
#ifndef PICO_I2C_DRIVER_H
#define PICO_I2C_DRIVER_H

#include <stdint.h>
#include <stddef.h>

#define PICO_I2C_ADDR      0x42
#define I2C_TIMEOUT_MS     25

/* Register addresses — matches Pico firmware register map */
#define REG_DEVICE_ID      0x00
#define REG_STATUS         0x01
#define REG_CH0_HI         0x02
#define REG_CH0_LO         0x03
#define REG_CH1_HI         0x04
#define REG_CH1_LO         0x05
#define REG_CH2_HI         0x06
#define REG_CH2_LO         0x07
#define REG_FAULT_INJECT   0x10
#define REG_FAULT_CLEAR    0x11
#define REG_SAMPLE_RATE    0x20

#define FAULT_I2C_TIMEOUT  0x01
#define EXPECTED_DEVICE_ID 0xA5

#define REG_ADDR_MIN       0x00
#define REG_ADDR_MAX       0x20

typedef enum {
    I2C_OK               =  0,
    I2C_ERR_NULL_HANDLE  = -1,
    I2C_ERR_INIT_FAILED  = -2,
    I2C_ERR_ADDR_INVALID = -3,
    I2C_ERR_TIMEOUT      = -4,
    I2C_ERR_NAK          = -5,
    I2C_ERR_IO           = -6,
    I2C_ERR_WRITE_DENIED = -7,
} PicoI2CStatus;

/* Opaque handle — struct body lives in pico_i2c_driver.c only.
   Callers may only hold a PicoI2CHandle* and must not dereference it. */
typedef struct PicoI2CHandle PicoI2CHandle;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Open the FT232H and configure MPSSE for I2C at 100kHz.
 * Returns a heap-allocated handle on success, NULL on failure.
 * Caller must pass the returned pointer to pico_i2c_close() when done.
 */
PicoI2CHandle* pico_i2c_open(void);

/**
 * Close device and free the handle.
 * Safe to call with NULL (no-op).
 */
void pico_i2c_close(PicoI2CHandle* handle);

/**
 * Read one byte from the Pico's register map.
 *
 * Guards detected by TestForge:
 *   handle == NULL          → I2C_ERR_NULL_HANDLE
 *   out == NULL             → I2C_ERR_NULL_HANDLE
 *   reg > REG_ADDR_MAX      → I2C_ERR_ADDR_INVALID
 *   I2C clock stretch >25ms → I2C_ERR_TIMEOUT
 *
 * @return I2C_OK on success, negative PicoI2CStatus on failure.
 */
PicoI2CStatus pico_i2c_read_register(PicoI2CHandle* handle,
                                      uint8_t        reg,
                                      uint8_t*       out);

/**
 * Write one byte to the Pico's register map.
 *
 * Guards detected by TestForge:
 *   handle == NULL               → I2C_ERR_NULL_HANDLE
 *   reg not in writable set      → I2C_ERR_WRITE_DENIED
 *   reg > REG_ADDR_MAX           → I2C_ERR_ADDR_INVALID
 *
 * Writable registers: REG_FAULT_INJECT, REG_FAULT_CLEAR, REG_SAMPLE_RATE
 */
PicoI2CStatus pico_i2c_write_register(PicoI2CHandle* handle,
                                       uint8_t        reg,
                                       uint8_t        value);

/**
 * Check whether the device is alive and responding.
 * Reads REG_DEVICE_ID and verifies it equals EXPECTED_DEVICE_ID (0xA5).
 * Returns I2C_OK if device responds correctly, error code otherwise.
 *
 * Guard detected by TestForge:
 *   handle == NULL → I2C_ERR_NULL_HANDLE
 */
PicoI2CStatus pico_i2c_ping(PicoI2CHandle* handle);

#ifdef __cplusplus
}
#endif

#endif /* PICO_I2C_DRIVER_H */
