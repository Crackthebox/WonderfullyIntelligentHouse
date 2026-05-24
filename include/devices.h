#ifndef DEVICES_H
#define DEVICES_H

#include <iostream>
#include <string>
#include <memory>
#include "exceptions.h"

class SmartDevice {
protected:
    std::string deviceName;
    bool isOn;
    static int totalDevices;
public:
    SmartDevice(const std::string& name, bool state);
    SmartDevice(const SmartDevice& other);
    virtual ~SmartDevice();

    static int getTotalDevices();
    virtual void turnOn();
    virtual void turnOff();
    bool getState() const;
    std::string getName() const;

    virtual void optimizeForClimate(double currentTemp, double desiredTemp) = 0;
    virtual std::shared_ptr<SmartDevice> clone() const = 0;
    virtual void print(std::ostream& os) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const SmartDevice& dev);
};

class LightSource : public SmartDevice {
private:
    int brightness;
public:
    explicit LightSource(bool state = false, int bright = 0);
    void setBrightness(int level);
    int getBrightness() const;
    void optimizeForClimate(double currentTemp, double desiredTemp) override;
    std::shared_ptr<SmartDevice> clone() const override;
    void print(std::ostream& os) const override;
};

class Heater : public SmartDevice {
public:
    explicit Heater(bool state = false);
    void optimizeForClimate(double currentTemp, double desiredTemp) override;
    std::shared_ptr<SmartDevice> clone() const override;
    void print(std::ostream& os) const override;
};

class AC : public SmartDevice {
public:
    explicit AC(bool state = false);
    void optimizeForClimate(double currentTemp, double desiredTemp) override;
    std::shared_ptr<SmartDevice> clone() const override;
    void print(std::ostream& os) const override;
};

class SmartBlinds : public SmartDevice {
public:
    explicit SmartBlinds(bool state = false);
    void optimizeForClimate(double currentTemp, double desiredTemp) override;
    std::shared_ptr<SmartDevice> clone() const override;
    void print(std::ostream& os) const override;
};

#endif