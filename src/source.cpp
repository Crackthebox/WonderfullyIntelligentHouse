#include "../include/source.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

LightSource::LightSource(bool state, int bright) : isOn(state), brightness(bright) {
}

LightSource::~LightSource() {
}

void LightSource::turnOn() { isOn = true; brightness = 100; }
void LightSource::turnOff() { isOn = false; brightness = 0; }
void LightSource::setBrightness(int level) { 
    brightness = level; 
    isOn = (brightness > 0);
}

std::ostream& operator<<(std::ostream& os, const LightSource& light) {
    os << "Lumina: " << (light.isOn ? "Aprinsa" : "Stinsa") << " (" << light.brightness << "%)";
    return os;
}

Heater::Heater(bool state) : isActive(state) {
}

Heater::~Heater() {
}

void Heater::turnOn() { isActive = true; }
void Heater::turnOff() { isActive = false; }

std::ostream& operator<<(std::ostream& os, const Heater& heater) {
    os << "Calorifer: " << (heater.isActive ? "Pornit" : "Oprit");
    return os;
}

AC::AC(bool state) : isActive(state) {
}

AC::~AC() {
}

void AC::turnOn() { isActive = true; }
void AC::turnOff() { isActive = false; }

std::ostream& operator<<(std::ostream& os, const AC& ac) {
    os << "AC: " << (ac.isActive ? "Pornit" : "Oprit");
    return os;
}

Thermostat::Thermostat(const char* model, double current, double desired) 
    : currentTemp(current), desiredTemp(desired) {
    modelName = new char[std::strlen(model) + 1];
    std::strcpy(modelName, model);
}

Thermostat::Thermostat(const Thermostat& other) 
    : currentTemp(other.currentTemp), desiredTemp(other.desiredTemp) {
    modelName = new char[std::strlen(other.modelName) + 1];
    std::strcpy(modelName, other.modelName);
}

Thermostat& Thermostat::operator=(const Thermostat& other) {
    if (this != &other) { 
        delete[] modelName; 
        currentTemp = other.currentTemp;
        desiredTemp = other.desiredTemp;
        modelName = new char[std::strlen(other.modelName) + 1];
        std::strcpy(modelName, other.modelName);
    }
    return *this;
}

Thermostat::~Thermostat() {
    delete[] modelName;
}

void Thermostat::setDesiredTemp(double temp) { desiredTemp = temp; }
void Thermostat::updateCurrentTemp(double temp) { currentTemp = temp; }
double Thermostat::getCurrentTemp() const { return currentTemp; }
double Thermostat::getDesiredTemp() const { return desiredTemp; }

std::ostream& operator<<(std::ostream& os, const Thermostat& term) {
    os << "Termostat [" << term.modelName << "] | Temp: " 
       << term.currentTemp << "C (Dorit: " << term.desiredTemp << "C)";
    return os;
}

Room::Room(std::string roomName, const Thermostat& term) 
    : name(roomName), thermostat(term) {
}

Room::~Room() {
}

void Room::regulateClimate() {
    double current = thermostat.getCurrentTemp();
    double desired = thermostat.getDesiredTemp();

    if (current < desired - 0.5) {
        heater.turnOn();
        ac.turnOff();
    } else if (current > desired + 0.5) {
        ac.turnOn();
        heater.turnOff();
    } else {
        heater.turnOff();
        ac.turnOff();
    }
}

void Room::setEcoMode() {
    light.turnOff();
    thermostat.setDesiredTemp(19.0);
    regulateClimate();
}

void Room::setComfortMode() {
    light.turnOn();
    thermostat.setDesiredTemp(23.0);
    regulateClimate();
}

void Room::changetemp(double t){
    thermostat.setDesiredTemp(t);
}
std::ostream& operator<<(std::ostream& os, const Room& room) {
    os << "Camera: " << room.name << "\n"
       << "  " << room.light << "\n"
       << "  " << room.thermostat << "\n"
       << "  " << room.heater << " | " << room.ac << "\n";
    return os;
}

House::House(std::string houseAddress) : address(houseAddress) {
}

House::~House() {
}
void House::printRoom(int x){
    std::cout<<rooms[x]<<"\n";
}
void House::addRoom(const Room& room) {
    rooms.push_back(room);
}

void House::enterHouse() {
    for (auto& room : rooms) {
        room.setComfortMode();
    }
}

void House::leaveHouse() {
    for (auto& room : rooms) {
        room.setEcoMode();
    }
}
void House::changetemp(double t, int nr){
    rooms[nr].changetemp(t);
}
Room& House::getroom(int index) {
    return rooms[index]; 
}
std::ostream& operator<<(std::ostream& os, const House& house) {
    os << "\n";
    os << "CASA: " << house.address << "\n";
    os << "\n";
    for (const auto& room : house.rooms) {
        os << room;
    }
    return os;
}