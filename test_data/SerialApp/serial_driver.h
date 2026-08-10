/**
 * TestForge™ — SerialApp Training Example
 * Embedded-style serial driver interface
 * © 2026 Myattech. All rights reserved.
 *
 * This file demonstrates the Discovery → Curation → Execution workflow.
 * Run: testforge discover -i test_data/SerialApp/
 */
#ifndef SERIAL_DRIVER_H
#define SERIAL_DRIVER_H

#include <stdint.h>
#include <stddef.h>

/* Maximum packet size for framed serial communication */
#define SERIAL_MAX_PACKET_SIZE  256
#define SERIAL_START_BYTE       0xAA
#define SERIAL_END_BYTE         0x55

/* Return codes */
typedef enum {
    SERIAL_OK        =  0,
    SERIAL_ERR_NULL  = -1,
    SERIAL_ERR_SIZE  = -2,
    SERIAL_ERR_FRAME = -3,
    SERIAL_ERR_CRC   = -4,
} SerialStatus;

/* Serial handle — opaque to callers */
typedef struct {
    uint32_t baud_rate;
    uint8_t  port_id;
    uint8_t  initialized;
    uint8_t  rx_buffer[SERIAL_MAX_PACKET_SIZE];
    uint16_t rx_count;
} SerialHandle;

/* === API functions — TestForge will discover and test these === */

SerialStatus serial_init(SerialHandle* handle, uint32_t baud_rate,
                         uint8_t port_id);

SerialStatus serial_send(SerialHandle* handle,
                         const uint8_t* data, uint16_t length);

SerialStatus serial_receive(SerialHandle* handle,
                            uint8_t* out_buf, uint16_t buf_size,
                            uint16_t* out_length);

uint8_t serial_compute_crc(const uint8_t* data, uint16_t length);

SerialStatus serial_send_framed(SerialHandle* handle,
                                const uint8_t* payload, uint16_t length);

SerialStatus serial_receive_framed(SerialHandle* handle,
                                   uint8_t* out_payload, uint16_t buf_size,
                                   uint16_t* out_length);

void serial_reset(SerialHandle* handle);

int serial_is_initialized(const SerialHandle* handle);

#endif /* SERIAL_DRIVER_H */
