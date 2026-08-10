/**
 * SensorAggregator.h
 *
 * C++ application layer for the Sensor Aggregator demo.
 * Wraps the pico_i2c_driver C HAL, collects channel readings, and
 * maintains a rolling average. All I2C calls go through the opaque
 * PicoI2CHandle*, keeping this class testable without hardware.
 *
 * Architecture:
 *   SensorAggregator (C++ class)
 *       └── PicoI2CHandle* (opaque C HAL handle)
 *               └── I2CTransport* (mocked in Phase 1, real FT232H in Phase 2)
 *
 * © 2026 Myattech LLC. All rights reserved.
 */
#ifndef SENSOR_AGGREGATOR_H
#define SENSOR_AGGREGATOR_H

#include <stdint.h>
#include <stddef.h>

/* Forward-declare the C HAL handle — avoids pulling pico_i2c_driver.h
   into every consumer of this header. */
struct PicoI2CHandle;

/* ---- Constants --------------------------------------------------------- */
#define SA_NUM_CHANNELS      3
#define SA_ROLLING_AVG_DEPTH 8

/* ---- Value types -------------------------------------------------------- */
typedef enum {
    SA_OK                  =  0,
    SA_NULL_ARG            = -1,
    SA_NOT_INITIALIZED     = -2,
    SA_CHANNEL_OUT_OF_RANGE = -3,
    SA_DATA_STALE          = -4,
    SA_I2C_FAULT           = -5,
    SA_DEVICE_NOT_FOUND    = -6,
} AggregatorStatus;

typedef struct {
    uint16_t raw;
    uint16_t rolling_avg;
    uint8_t  valid;
} ChannelReading;

/* ---- Class -------------------------------------------------------------- */
class SensorAggregator {
public:
    /**
     * Construct with an I2C driver handle.
     * handle may be nullptr — all operations will return SA_NOT_INITIALIZED
     * until a valid handle is provided and initialize() succeeds.
     */
    explicit SensorAggregator(struct PicoI2CHandle* handle = nullptr);

    ~SensorAggregator();

    /** Ping the device. Returns SA_OK on success. */
    AggregatorStatus initialize();

    /** Read all three channels and update rolling averages. */
    AggregatorStatus poll();

    /** Copy the latest reading for channel [0..SA_NUM_CHANNELS-1] into *out. */
    AggregatorStatus get_channel(int channel, ChannelReading* out) const;

    /** Write fault_code to REG_FAULT_INJECT. Requires initialize() first. */
    AggregatorStatus inject_fault(uint8_t fault_code);

    /** Write 0xFF to REG_FAULT_CLEAR. Requires initialize() first. */
    AggregatorStatus clear_fault();

    bool is_faulted()     const;
    bool is_initialized() const;

private:
    /** Read raw 16-bit ADC value for one channel over I2C. */
    AggregatorStatus read_channel_raw(int ch, uint16_t* out);

    /** Compute rolling average for channel ch from stored history. */
    uint16_t compute_rolling_avg(int ch) const;

    struct PicoI2CHandle* handle_;
    bool                  initialized_;
    bool                  faulted_;
    ChannelReading        channels_[SA_NUM_CHANNELS];
    uint16_t              history_[SA_NUM_CHANNELS][SA_ROLLING_AVG_DEPTH];
    int                   history_count_[SA_NUM_CHANNELS];
    int                   history_idx_;
};

#endif /* SENSOR_AGGREGATOR_H */
