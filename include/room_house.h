#ifndef ROOM_HOUSE_H
#define ROOM_HOUSE_H

#include <vector>
#include <string>
#include <memory>
#include "Devices.h"
#include "Templates.h"

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

    void simulationTick(double outsideTemp, bool comfortMode);

    std::string getName() const { return name; }
    Thermostat getThermostat() const { return thermostat; }
    const std::vector<std::shared_ptr<SmartDevice>>& getDevices() const { return devices; }

    friend std::ostream& operator<<(std::ostream& os, const Room& room);
};

class House {
private:
    std::string address;
    std::vector<Room> rooms;
    double outsideTemp;
    bool comfortModeActive;

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

    void simulationStep();
    void setOutsideTemp(double t) { outsideTemp = t; }
    double getOutsideTemp() const { return outsideTemp; }
    bool isComfortMode() const { return comfortModeActive; }
    
    friend std::ostream& operator<<(std::ostream& os, const House& house);
};

#endif