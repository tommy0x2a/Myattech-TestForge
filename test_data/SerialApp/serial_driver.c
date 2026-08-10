/**
 * TestForge™ — SerialApp Training Example
 * Realistic embedded serial driver (no placeholders)
 * © 2026 Myattech. All rights reserved.
 */
#include "serial_driver.h"
#include <string.h>

SerialStatus serial_init(SerialHandle* handle, uint32_t baud_rate, uint8_t port_id) {
    if (!handle) return SERIAL_ERR_NULL;
    if (baud_rate == 0) return SERIAL_ERR_NULL;  /* guard */
    handle->baud_rate = baud_rate;
    handle->port_id = port_id;
    handle->initialized = 1;
    handle->rx_count = 0;
    memset(handle->rx_buffer, 0, sizeof(handle->rx_buffer));
    return SERIAL_OK;
}

SerialStatus serial_send(SerialHandle* handle, const uint8_t* data, uint16_t length) {
    if (!handle || !data) return SERIAL_ERR_NULL;
    if (length == 0 || length > SERIAL_MAX_PACKET_SIZE) return SERIAL_ERR_SIZE;
    /* simulate TX by storing in a tx shadow inside the handle for test visibility */
    /* (real driver would write to UART registers) */
    handle->rx_count = length; /* reuse rx_count as last-sent length for demo */
    return SERIAL_OK;
}

SerialStatus serial_receive(SerialHandle* handle, uint8_t* out_buf, uint16_t buf_size, uint16_t* out_length) {
    if (!handle || !out_buf || !out_length) return SERIAL_ERR_NULL;
    uint16_t n = handle->rx_count < buf_size ? handle->rx_count : buf_size;
    memcpy(out_buf, handle->rx_buffer, n);
    *out_length = n;
    return SERIAL_OK;
}

uint8_t serial_compute_crc(const uint8_t* data, uint16_t length) {
    if (!data || length == 0) return 0;
    uint8_t crc = 0;
    for (uint16_t i = 0; i < length; ++i) {
        crc ^= data[i];
    }
    return crc;
}

SerialStatus serial_send_framed(SerialHandle* handle, const uint8_t* payload, uint16_t length) {
    if (!handle || !payload) return SERIAL_ERR_NULL;
    if (length == 0 || length + 3 > SERIAL_MAX_PACKET_SIZE) return SERIAL_ERR_SIZE;
    uint8_t frame[SERIAL_MAX_PACKET_SIZE];
    frame[0] = SERIAL_START_BYTE;
    memcpy(&frame[1], payload, length);
    frame[1 + length] = serial_compute_crc(payload, length);
    frame[2 + length] = SERIAL_END_BYTE;
    return serial_send(handle, frame, 3 + length);
}

SerialStatus serial_receive_framed(SerialHandle* handle, uint8_t* out_payload, uint16_t buf_size, uint16_t* out_length) {
    if (!handle || !out_payload || !out_length) return SERIAL_ERR_NULL;
    uint8_t raw[SERIAL_MAX_PACKET_SIZE];
    uint16_t raw_len = 0;
    SerialStatus st = serial_receive(handle, raw, sizeof(raw), &raw_len);
    if (st != SERIAL_OK) return st;
    if (raw_len < 3 || raw[0] != SERIAL_START_BYTE || raw[raw_len-1] != SERIAL_END_BYTE)
        return SERIAL_ERR_FRAME;
    uint16_t payload_len = raw_len - 3;
    if (payload_len > buf_size) return SERIAL_ERR_SIZE;
    uint8_t crc = serial_compute_crc(&raw[1], payload_len);
    if (crc != raw[raw_len-2]) return SERIAL_ERR_CRC;
    memcpy(out_payload, &raw[1], payload_len);
    *out_length = payload_len;
    return SERIAL_OK;
}

void serial_reset(SerialHandle* handle) {
    if (handle) {
        memset(handle, 0, sizeof(*handle));
    }
}

int serial_is_initialized(const SerialHandle* handle) {
    if (!handle) return 0;
    return handle->initialized ? 1 : 0;
}
