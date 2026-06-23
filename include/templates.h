#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <vector>
#include <memory>
#include <string>
#include <iostream>

template <typename T>
class Sensor {
private:
    std::string name;
    T value;
public:
    Sensor(std::string n, T initVal) : name(n), value(initVal) {}
    T read() const { return value; }
    void update(T newVal) { value = newVal; }
    std::string getName() const { return name; }
    void printInfo() const { std::cout << "[Senzor] " << name << ": " << value << "\n"; }
};

template <typename TargetType, typename BaseType>
std::vector<std::shared_ptr<TargetType>> filterDevicesByType(const std::vector<std::shared_ptr<BaseType>>& devices) {
    std::vector<std::shared_ptr<TargetType>> result;
    for (const auto& dev : devices) {
        if (auto casted = std::dynamic_pointer_cast<TargetType>(dev)) {
            result.push_back(casted);
        }
    }
    return result;
}

#endif