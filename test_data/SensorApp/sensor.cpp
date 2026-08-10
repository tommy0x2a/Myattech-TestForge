/**
 * sensor.cpp
 * Realistic embedded C++ temperature sensor driver for an STM32-based system.
 */

#include <cstdint>
#include <cstring>

namespace embedded {

class TemperatureSensor {
public:
    TemperatureSensor(uint8_t address, float scale_factor);
    ~TemperatureSensor();

    float readTemperature();
    bool isConnected();
    void setCalibration(float offset, float gain);

    static TemperatureSensor* getInstance();

private:
    uint16_t readRawADC();

    uint8_t i2c_address;
    float scale;
    float offset;
    float gain;
    bool connected;
};

TemperatureSensor::TemperatureSensor(uint8_t address, float scale_factor)
    : i2c_address(address), scale(scale_factor), offset(0.0f), gain(1.0f), connected(false) {}

TemperatureSensor::~TemperatureSensor() {}

float TemperatureSensor::readTemperature() {
    uint16_t raw = readRawADC();
    float temp = (raw / 65535.0f) * scale + offset;
    return temp * gain;
}

bool TemperatureSensor::isConnected() {
    return connected;
}

void TemperatureSensor::setCalibration(float off, float g) {
    offset = off;
    gain = g;
}

uint16_t TemperatureSensor::readRawADC() {
    // Simulated I2C read
    return 0x1234;
}

TemperatureSensor* TemperatureSensor::getInstance() {
    static TemperatureSensor instance(0x48, 100.0f);
    return &instance;
}

} // namespace embedded

float celsius_to_fahrenheit(float celsius) {
    return (celsius * 9.0f / 5.0f) + 32.0f;
}
