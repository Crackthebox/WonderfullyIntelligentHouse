#include "devices.h"

int SmartDevice::totalDevices = 0;

SmartDevice::SmartDevice(const std::string& name, bool state) : deviceName(name), isOn(state) { totalDevices++; }
SmartDevice::SmartDevice(const SmartDevice& other) : deviceName(other.deviceName), isOn(other.isOn) { totalDevices++; }
SmartDevice::~SmartDevice() { totalDevices--; }
int SmartDevice::getTotalDevices() { return totalDevices; }
void SmartDevice::turnOn() { isOn = true; }
void SmartDevice::turnOff() { isOn = false; }
bool SmartDevice::getState() const { return isOn; }
std::string SmartDevice::getName() const { return deviceName; }

std::ostream& operator<<(std::ostream& os, const SmartDevice& dev) { dev.print(os); return os; }

LightSource::LightSource(bool state, int bright) : SmartDevice("Lumina", state), brightness(bright) {}
void LightSource::setBrightness(int level) { 
    if (level < 0 || level > 100) throw InvalidBrightnessException(level);
    isOn = (level > 0); brightness = level;
}
int LightSource::getBrightness() const { return brightness; }
void LightSource::optimizeForClimate(double, double) {}
std::shared_ptr<SmartDevice> LightSource::clone() const { return std::make_shared<LightSource>(*this); }
void LightSource::print(std::ostream& os) const { os << deviceName << ": " << (isOn ? "Aprinsa" : "Stinsa") << " (" << brightness << "%)"; }

Heater::Heater(bool state) : SmartDevice("Calorifer", state) {}
void Heater::optimizeForClimate(double currentTemp, double desiredTemp) {
    if (currentTemp < desiredTemp - 0.5) turnOn(); else turnOff();
}
std::shared_ptr<SmartDevice> Heater::clone() const { return std::make_shared<Heater>(*this); }
void Heater::print(std::ostream& os) const { os << deviceName << ": " << (isOn ? "Pornit" : "Oprit"); }

AC::AC(bool state) : SmartDevice("Aer Conditionat", state) {}
void AC::optimizeForClimate(double currentTemp, double desiredTemp) {
    if (currentTemp > desiredTemp + 0.5) turnOn(); else turnOff();
}
std::shared_ptr<SmartDevice> AC::clone() const { return std::make_shared<AC>(*this); }
void AC::print(std::ostream& os) const { os << deviceName << ": " << (isOn ? "Pornit" : "Oprit"); }

SmartBlinds::SmartBlinds(bool state) : SmartDevice("Jaluzele", state) {}
void SmartBlinds::optimizeForClimate(double currentTemp, double desiredTemp) {
    if (currentTemp > desiredTemp + 1.5) turnOff(); else turnOn();
}
std::shared_ptr<SmartDevice> SmartBlinds::clone() const { return std::make_shared<SmartBlinds>(*this); }
void SmartBlinds::print(std::ostream& os) const { os << deviceName << ": " << (isOn ? "Ridicate" : "Coborate"); }