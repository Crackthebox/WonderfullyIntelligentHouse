#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class SmartHomeException : public std::exception {
protected:
    std::string message;
public:
    explicit SmartHomeException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InvalidTemperatureException : public SmartHomeException {
public:
    explicit InvalidTemperatureException(double temp) 
        : SmartHomeException("Eroare: Temperatura " + std::to_string(temp) + " este invalida! (Permis: 10.0 - 35.0 C)") {}
};

class RoomNotFoundException : public SmartHomeException {
public:
    explicit RoomNotFoundException(int index)
        : SmartHomeException("Eroare: Camera cu indexul " + std::to_string(index) + " nu exista!") {}
};

class InvalidBrightnessException : public SmartHomeException {
public:
    explicit InvalidBrightnessException(int level)
        : SmartHomeException("Eroare: Luminozitatea " + std::to_string(level) + " este invalida! (Permis: 0 - 100)") {}
};

class HouseNotFoundException : public SmartHomeException {
public:
    explicit HouseNotFoundException(int index)
        : SmartHomeException("Eroare: Casa cu indexul " + std::to_string(index) + " nu exista!") {}
};

#endif