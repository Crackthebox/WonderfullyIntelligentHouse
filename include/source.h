#include <iostream>
#include <string>
#include <vector>

class LightSource {
private:
    bool isOn;
    int brightness;

public:
    LightSource(bool state = false, int bright = 0);
    ~LightSource(); 
    void turnOn();
    void turnOff();
    void setBrightness(int level);
    
    friend std::ostream& operator<<(std::ostream& os, const LightSource& light);
};

class Heater {
private:
    bool isActive;

public:
    Heater(bool state = false);
    ~Heater(); 
    
    void turnOn();
    void turnOff();
    
    friend std::ostream& operator<<(std::ostream& os, const Heater& heater);
};

class AC {
private:
    bool isActive;

public:
    AC(bool state = false);
    ~AC();
    
    void turnOn();
    void turnOff();
    
    friend std::ostream& operator<<(std::ostream& os, const AC& ac);
};

class Thermostat {
private:
    char* modelName;
    double currentTemp;
    double desiredTemp;

public:
    Thermostat(const char* model = "Generic", double current = 20.0, double desired = 22.0);
    
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
    LightSource light;
    Heater heater;
    AC ac;
    Thermostat thermostat;

public:
    Room(std::string roomName, const Thermostat& term);
    ~Room(); 
    
    void regulateClimate(); 
    void setEcoMode();
    void setComfortMode();
    void changetemp(double t);
    friend std::ostream& operator<<(std::ostream& os, const Room& room);
};

class House {
private:
    std::string address;
    std::vector<Room> rooms;

public:
    House(std::string houseAddress);
    ~House(); 
    void printRoom(int x);
    void addRoom(const Room& room);
    void enterHouse();
    void leaveHouse();
    void changetemp(double t, int nr);
    Room& getroom(int index);
    friend std::ostream& operator<<(std::ostream& os, const House& house);
};