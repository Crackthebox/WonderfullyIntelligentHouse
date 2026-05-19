#ifndef SOURCE_H
#define SOURCE_H

#include <iostream>
#include <string>
#include <vector>
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

class Thermostat {
private:
    char* modelName;
    double currentTemp;
    double desiredTemp;
public:
    explicit Thermostat(const char* model = "Generic", double current = 20.0, double desired = 22.0);
    Thermostat(const Thermostat& other);           
    Thermostat& operator=(const Thermostat& other); 
    ~Thermostat();                                  

    void setDesiredTemp(double temp);
    void updateCurrentTemp(double temp);
    double getCurrentTemp() const;
    double getDesiredTemp() const;
    
    friend std::ostream& operator<<(std::ostream& os, const Thermostat& term);
};

class Room {
private:
    std::string name;
    Thermostat thermostat;
    std::vector<std::shared_ptr<SmartDevice>> devices;

public:
    Room(std::string roomName, const Thermostat& term);
    Room(const Room& other);
    Room& operator=(const Room& other);
    ~Room() = default;
    
    void addDevice(std::shared_ptr<SmartDevice> device);
    void regulateClimate(); 
    void setEcoMode();
    void setComfortMode();
    void changetemp(double t);
    void forceLightsBrightness(int level);

    friend std::ostream& operator<<(std::ostream& os, const Room& room);
};

class House {
private:
    std::string address;
    std::vector<Room> rooms;

public:
    explicit House(const std::string& houseAddress);
    ~House() = default; 
    
    std::string getAddress() const;
    void addRoom(const Room& room);
    void enterHouse();
    void leaveHouse();
    void changetemp(double t, int nr);
    Room& getroom(int index);
    int getRoomCount() const;
    
    friend std::ostream& operator<<(std::ostream& os, const House& house);
};

#endif