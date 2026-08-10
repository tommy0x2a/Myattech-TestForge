// test_data/StressTestApp/stress_cpp.cpp
// C++ stress test cases for TestForge (classes, templates, inheritance, etc.)

#include <cstdint>
#include <cstddef>

namespace embedded {

template<typename T, size_t N>
class CircularBuffer {
public:
    CircularBuffer() : head(0), tail(0), full(false) {}

    bool push(const T& item) {
        if (full) return false;
        buffer[head] = item;
        head = (head + 1) % N;
        full = (head == tail);
        return true;
    }

    bool pop(T& item) {
        if (empty()) return false;
        item = buffer[tail];
        tail = (tail + 1) % N;
        full = false;
        return true;
    }

    bool empty() const { return (!full && (head == tail)); }

private:
    T buffer[N];
    size_t head;
    size_t tail;
    bool full;
};

// === Base class with virtual functions ===
class Sensor {
public:
    virtual ~Sensor() = default;
    virtual float read() = 0;
    virtual bool isConnected() const = 0;
};

// === Derived class ===
class TemperatureSensor : public Sensor {
public:
    TemperatureSensor(uint8_t address) : i2c_address(address), connected(false) {}

    float read() override {
        if (!connected) return -273.15f;
        return readRawTemperature();
    }

    bool isConnected() const override { return connected; }

private:
    float readRawTemperature() {
        // Simulated I2C read
        return 23.5f;
    }

    uint8_t i2c_address;
    bool connected;
};

// === RAII-style resource manager ===
class PeripheralLock {
public:
    explicit PeripheralLock(uint32_t peripheral_id) : id(peripheral_id), locked(false) {
        lock();
    }

    ~PeripheralLock() {
        if (locked) unlock();
    }

    bool isLocked() const { return locked; }

private:
    void lock()   { locked = true; }
    void unlock() { locked = false; }

    uint32_t id;
    bool locked;
};

// === Template function with complex parameters ===
template<typename T>
bool process_sensor_data(T& sensor, CircularBuffer<float, 16>& buffer) {
    if (!sensor.isConnected()) return false;

    float value = sensor.read();
    return buffer.push(value);
}

} // namespace embedded