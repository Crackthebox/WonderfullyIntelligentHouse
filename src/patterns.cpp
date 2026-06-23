#include "patterns.h"
#include "devices.h"

std::shared_ptr<SmartDevice> DeviceFactory::createDevice(const std::string& type) {
    if (type == "Lumina") return std::make_shared<LightSource>();
    if (type == "Calorifer") return std::make_shared<Heater>();
    if (type == "Aer Conditionat") return std::make_shared<AC>();
    if (type == "Jaluzele") return std::make_shared<SmartBlinds>();
    return nullptr;
}