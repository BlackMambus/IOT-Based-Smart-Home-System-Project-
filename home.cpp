#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <map>

// Simulated MQTT message handler
void sendMQTTMessage(const std::string& topic, const std::string& message) {
    std::cout << "[MQTT] Topic: " << topic << " | Message: " << message << "\n";
}

// Simulated temperature and humidity sensor
class EnvironmentSensor {
public:
    float getTemperature() {
        return 20.0f + static_cast<float>(rand() % 1000) / 100.0f; // 20.0 - 30.0°C
    }

    float getHumidity() {
        return 40.0f + static_cast<float>(rand() % 2000) / 100.0f; // 40.0 - 60.0%
    }
};

// Smart light
class SmartLight {
    bool isOn = false;
public:
    void turnOn() {
        isOn = true;
        sendMQTTMessage("home/light/status", "ON");
    }

    void turnOff() {
        isOn = false;
        sendMQTTMessage("home/light/status", "OFF");
    }

    bool status() const { return isOn; }
};

// Smart door lock
class SmartLock {
    bool isLocked = true;
public:
    void lock() {
        isLocked = true;
        sendMQTTMessage("home/door/status", "LOCKED");
    }

    void unlock() {
        isLocked = false;
        sendMQTTMessage("home/door/status", "UNLOCKED");
    }

    bool status() const { return isLocked; }
};

// Central controller
class SmartHomeController {
    EnvironmentSensor sensor;
    SmartLight light;
    SmartLock lock;

public:
    void monitorEnvironment() {
        float temp = sensor.getTemperature();
        float humidity = sensor.getHumidity();

        sendMQTTMessage("home/env/temperature", std::to_string(temp));
        sendMQTTMessage("home/env/humidity", std::to_string(humidity));

        // Auto control logic
        if (temp > 28.0f) {
            light.turnOn();
        } else {
            light.turnOff();
        }
    }

    void handleCommand(const std::string& topic, const std::string& payload) {
        if (topic == "home/door/command") {
            if (payload == "LOCK") lock.lock();
            else if (payload == "UNLOCK") lock.unlock();
        } else if (topic == "home/light/command") {
            if (payload == "ON") light.turnOn();
            else if (payload == "OFF") light.turnOff();
        }
    }

    void run() {
        for (int i = 0; i < 5; ++i) {
            monitorEnvironment();
           
        }

        // Simulate external commands
        handleCommand("home/door/command", "UNLOCK");
        handleCommand("home/light/command", "OFF");
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    SmartHomeController controller;
    controller.run();
    return 0;
}

