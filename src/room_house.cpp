#include "room_house.h"
#include <cstring>
#include "exceptions.h"

Thermostat::Thermostat(const char* model, double current, double desired) 
    : currentTemp(current), desiredTemp(desired) {
    modelName = new char[std::strlen(model) + 1]; std::strcpy(modelName, model);
}
Thermostat::Thermostat(const Thermostat& other) : currentTemp(other.currentTemp), desiredTemp(other.desiredTemp) {
    modelName = new char[std::strlen(other.modelName) + 1]; std::strcpy(modelName, other.modelName);
}
Thermostat& Thermostat::operator=(const Thermostat& other) {
    if (this != &other) { 
        delete[] modelName; 
        currentTemp = other.currentTemp; desiredTemp = other.desiredTemp;
        modelName = new char[std::strlen(other.modelName) + 1]; std::strcpy(modelName, other.modelName);
    }
    return *this;
}
Thermostat::~Thermostat() { delete[] modelName; }
void Thermostat::setDesiredTemp(double temp) { if (temp < 10.0 || temp > 35.0) throw InvalidTemperatureException(temp); desiredTemp = temp; }
void Thermostat::updateCurrentTemp(double temp) { currentTemp = temp; }
double Thermostat::getCurrentTemp() const { return currentTemp; }
double Thermostat::getDesiredTemp() const { return desiredTemp; }
std::ostream& operator<<(std::ostream& os, const Thermostat& term) {
    os << "Termostat [" << term.modelName << "] | Temp: " << term.currentTemp << "C (Dorit: " << term.desiredTemp << "C)"; return os;
}

Room::Room(std::string roomName, const Thermostat& term) : name(std::move(roomName)), thermostat(term) {}
Room::Room(const Room& other) : name(other.name), thermostat(other.thermostat) {
    for (const auto& dev : other.devices) devices.push_back(dev->clone());
}
Room& Room::operator=(const Room& other) {
    if (this != &other) {
        name = other.name; thermostat = other.thermostat; devices.clear();
        for (const auto& dev : other.devices) devices.push_back(dev->clone());
    }
    return *this;
}
void Room::addDevice(std::shared_ptr<SmartDevice> device) { devices.push_back(device); }
void Room::regulateClimate() {
    double current = thermostat.getCurrentTemp(); double desired = thermostat.getDesiredTemp();
    for (auto& dev : devices) dev->optimizeForClimate(current, desired);
}
void Room::setEcoMode() {
    thermostat.setDesiredTemp(16.0);
    for (auto& dev : devices) dev->turnOff();
}
void Room::setComfortMode() {
    thermostat.setDesiredTemp(22.0); forceLightsBrightness(80); regulateClimate();
}
void Room::changetemp(double t) { thermostat.setDesiredTemp(t); }
void Room::forceLightsBrightness(int level) {
    for (auto& dev : devices) {
        std::shared_ptr<LightSource> light = std::dynamic_pointer_cast<LightSource>(dev);
        if (light) light->setBrightness(level);
    }
}

void Room::simulationTick(double outsideTemp, bool) {
    double current = thermostat.getCurrentTemp();
    bool roomIsActive = false;
    
    for (const auto& dev : devices) {
        if (dev->getName() == "Lumina" && dev->getState()) roomIsActive = true;
    }

    if (roomIsActive) {
        regulateClimate(); 
    } else {
        for (auto& dev : devices) {
            if (dev->getName() != "Lumina") dev->turnOff();
        }
    }

    bool heaterOn = false, acOn = false;
    for (const auto& dev : devices) {
        if (dev->getName() == "Calorifer" && dev->getState()) heaterOn = true;
        if (dev->getName() == "Aer Conditionat" && dev->getState()) acOn = true;
    }
    
    if (heaterOn) current += 0.8;
    if (acOn) current -= 0.8;
    
    current += (outsideTemp - current) * 0.05;

    thermostat.updateCurrentTemp(current);
}

std::ostream& operator<<(std::ostream& os, const Room& room) {
    os << "Camera: " << room.name << "\n  " << room.thermostat << "\n";
    for (const auto& dev : room.devices) os << "  - " << *dev << "\n";
    return os;
}

House::House(const std::string& houseAddress) 
    : address(houseAddress), outsideTemp(10.0), comfortModeActive(false) {}

std::string House::getAddress() const { return address; }
void House::addRoom(const Room& room) { rooms.push_back(room); }
void House::enterHouse() { 
    comfortModeActive = true; 
    for (auto& room : rooms) room.setComfortMode(); 
}
void House::leaveHouse() { 
    comfortModeActive = false; 
    for (auto& room : rooms) room.setEcoMode(); 
}
void House::changetemp(double t, int nr) { if (nr < 0 || nr >= static_cast<int>(rooms.size())) throw RoomNotFoundException(nr); rooms[nr].changetemp(t); }
Room& House::getroom(int index) { if (index < 0 || index >= static_cast<int>(rooms.size())) throw RoomNotFoundException(index); return rooms[index]; }
int House::getRoomCount() const { return static_cast<int>(rooms.size()); }

void House::simulationStep() {
    for (auto& room : rooms) {
        room.simulationTick(outsideTemp, comfortModeActive);
    }
}

std::ostream& operator<<(std::ostream& os, const House& house) {
    os << "\n=== CASA: " << house.address << " ===\n";
    for (const auto& room : house.rooms) os << room << "\n";
    return os;
}