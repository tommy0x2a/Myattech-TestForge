/**
 * SensorAggregator.cpp
 *
 * © 2026 Myattech LLC. All rights reserved.
 */

#include "SensorAggregator.h"
#include "pico_i2c_driver.h"
#include <cstring>

SensorAggregator::SensorAggregator(struct PicoI2CHandle* handle)
    : handle_(handle)
    , initialized_(false)
    , faulted_(false)
    , history_idx_(0)
{
    memset(channels_,      0, sizeof(channels_));
    memset(history_,       0, sizeof(history_));
    memset(history_count_, 0, sizeof(history_count_));
}

SensorAggregator::~SensorAggregator() {
    /* handle_ lifetime is managed by the caller */
}

AggregatorStatus SensorAggregator::initialize() {
    if (!handle_)
        return SA_NOT_INITIALIZED;

    PicoI2CStatus st = pico_i2c_ping(handle_);
    if (st == I2C_OK) {
        initialized_ = true;
        return SA_OK;
    }
    if (st == I2C_ERR_NULL_HANDLE)
        return SA_NOT_INITIALIZED;
    if (st == I2C_ERR_NAK)
        return SA_DEVICE_NOT_FOUND;
    return SA_I2C_FAULT;
}

AggregatorStatus SensorAggregator::read_channel_raw(int ch, uint16_t* out) {
    if (ch < 0 || ch >= SA_NUM_CHANNELS) return SA_CHANNEL_OUT_OF_RANGE;
    if (!out) return SA_NULL_ARG;

    static const uint8_t hi_regs[SA_NUM_CHANNELS] = {
        REG_CH0_HI, REG_CH1_HI, REG_CH2_HI
    };
    static const uint8_t lo_regs[SA_NUM_CHANNELS] = {
        REG_CH0_LO, REG_CH1_LO, REG_CH2_LO
    };

    uint8_t hi = 0, lo = 0;

    PicoI2CStatus st = pico_i2c_read_register(handle_, hi_regs[ch], &hi);
    if (st != I2C_OK) {
        if (st == I2C_ERR_NULL_HANDLE) return SA_NOT_INITIALIZED;
        return SA_I2C_FAULT;
    }
    st = pico_i2c_read_register(handle_, lo_regs[ch], &lo);
    if (st != I2C_OK) {
        if (st == I2C_ERR_NULL_HANDLE) return SA_NOT_INITIALIZED;
        return SA_I2C_FAULT;
    }

    *out = static_cast<uint16_t>((hi << 8) | lo);
    return SA_OK;
}

uint16_t SensorAggregator::compute_rolling_avg(int ch) const {
    if (ch < 0 || ch >= SA_NUM_CHANNELS) return 0;
    int count = history_count_[ch];
    if (count == 0) return 0;
    uint32_t sum = 0;
    for (int i = 0; i < count; i++)
        sum += history_[ch][i];
    return static_cast<uint16_t>(sum / static_cast<uint32_t>(count));
}

AggregatorStatus SensorAggregator::poll() {
    if (!initialized_)
        return SA_DATA_STALE;

    for (int ch = 0; ch < SA_NUM_CHANNELS; ch++) {
        uint16_t raw = 0;
        AggregatorStatus st = read_channel_raw(ch, &raw);
        if (st != SA_OK) {
            faulted_ = true;
            return SA_I2C_FAULT;
        }
        history_[ch][history_idx_] = raw;
        if (history_count_[ch] < SA_ROLLING_AVG_DEPTH)
            history_count_[ch]++;
        channels_[ch].raw         = raw;
        channels_[ch].rolling_avg = compute_rolling_avg(ch);
        channels_[ch].valid       = 1;
    }

    history_idx_ = (history_idx_ + 1) % SA_ROLLING_AVG_DEPTH;
    faulted_     = false;
    return SA_OK;
}

AggregatorStatus SensorAggregator::get_channel(int channel, ChannelReading* out) const {
    if (!out)
        return SA_NULL_ARG;
    if (channel < 0 || channel >= SA_NUM_CHANNELS)
        return SA_CHANNEL_OUT_OF_RANGE;
    if (!channels_[channel].valid)
        return SA_DATA_STALE;
    *out = channels_[channel];
    return SA_OK;
}

AggregatorStatus SensorAggregator::inject_fault(uint8_t fault_code) {
    if (!initialized_)
        return SA_NOT_INITIALIZED;
    PicoI2CStatus st = pico_i2c_write_register(handle_, REG_FAULT_INJECT, fault_code);
    if (st == I2C_OK) return SA_OK;
    if (st == I2C_ERR_NULL_HANDLE) return SA_NOT_INITIALIZED;
    return SA_I2C_FAULT;
}

AggregatorStatus SensorAggregator::clear_fault() {
    if (!initialized_)
        return SA_NOT_INITIALIZED;
    PicoI2CStatus st = pico_i2c_write_register(handle_, REG_FAULT_CLEAR, 0xFF);
    if (st == I2C_OK) {
        faulted_ = false;
        return SA_OK;
    }
    return SA_I2C_FAULT;
}

bool SensorAggregator::is_faulted() const {
    return faulted_;
}

bool SensorAggregator::is_initialized() const {
    return initialized_;
}
