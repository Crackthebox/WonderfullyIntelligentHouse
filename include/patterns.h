#ifndef PATTERNS_H
#define PATTERNS_H

#include <string>
#include <iostream>
#include <memory>

class Logger {
private:
    Logger() = default;
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const std::string& message) { std::cout << "[INFO] " << message << "\n"; }
    void error(const std::string& message) { std::cerr << "[ERROR] " << message << "\n"; }
};

class SmartDevice;

class DeviceFactory {
public:
    static std::shared_ptr<SmartDevice> createDevice(const std::string& type);
};

#endif