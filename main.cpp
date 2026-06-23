#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "exceptions.h"
#include "templates.h"
#include "patterns.h"
#include "devices.h"
#include "room_house.h"

struct RoomVisual { int gridX; int gridY; };

RoomVisual getNewRoomPosition(const std::vector<RoomVisual>& existing) {
    if (existing.empty()) return {0, 0};
    std::vector<sf::Vector2i> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    while (true) {
        RoomVisual base = existing[rand() % existing.size()];
        sf::Vector2i dir = directions[rand() % 4];
        RoomVisual cand = {base.gridX + dir.x, base.gridY + dir.y};
        bool ok = true;
        for (const auto& ex : existing) {
            if (ex.gridX == cand.gridX && ex.gridY == cand.gridY) { ok = false; break; }
        }
        if (ok) return cand;
    }
}

struct Button { sf::RectangleShape rect; sf::Text text; int id; };

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    
    Sensor<double> outsideTempSensor("Termometru Exterior", 10.0);
    Sensor<bool> rainSensor("Senzor Ploaie", false);
    
    Logger::getInstance().log("Pornire Simulator Smart Home...");
    
    House myHouse("Casa Dinamica POO");
    std::vector<RoomVisual> roomVisuals;

    std::ifstream fin("config_casa.txt");
    if (fin.is_open()) {
        int numHouses;
        if (fin >> numHouses) {
            fin.ignore(1000, '\n');
            std::string address; std::getline(fin, address);
            int numRooms; fin >> numRooms;
            for (int i = 0; i < numRooms; ++i) {
                std::string rName; double cTemp, dTemp;
                fin >> rName >> cTemp >> dTemp;
                Room r(rName, Thermostat("Generic", cTemp, dTemp));
                
                r.addDevice(DeviceFactory::createDevice("Lumina"));
                r.addDevice(DeviceFactory::createDevice("Calorifer"));
                r.addDevice(DeviceFactory::createDevice("Aer Conditionat"));
                r.addDevice(DeviceFactory::createDevice("Jaluzele"));
                
                myHouse.addRoom(r);
                roomVisuals.push_back(getNewRoomPosition(roomVisuals));
            }
        }
        fin.close();
    } else {
        Room r1("Living", Thermostat("Model-X", 25.0, 22.0));
        r1.addDevice(DeviceFactory::createDevice("Lumina"));
        r1.addDevice(DeviceFactory::createDevice("Calorifer"));
        r1.addDevice(DeviceFactory::createDevice("Aer Conditionat"));
        myHouse.addRoom(r1);
        roomVisuals.push_back({0, 0});
    }

    sf::RenderWindow window(sf::VideoMode(1200, 800), "Smart Home Real-Time Simulator");
    window.setFramerateLimit(60);
    sf::Font font; font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

    sf::View houseView(sf::FloatRect(0.f, 0.f, 900.f, 800.f));
    houseView.setViewport(sf::FloatRect(0.f, 0.f, 0.75f, 1.f));

    std::vector<Button> buttons;
    std::vector<std::string> btnLabels = {
        "Adauga Camera", "Intra in Casa (Comfort)", "Iesi din Casa (Eco)", 
        "Regleaza Clima Automat", "Temp Exterior +5 C", "Temp Exterior -5 C",
        "Tinta Termica +1 C", "Tinta Termica -1 C", "Print in Consola", "Iesire"
    };
    
    for (size_t i = 0; i < btnLabels.size(); ++i) {
        Button b; b.rect.setSize(sf::Vector2f(260, 45)); b.rect.setPosition(920, 100 + i * 60);
        b.rect.setFillColor(sf::Color(45, 52, 71)); b.rect.setOutlineThickness(2);
        b.rect.setOutlineColor(sf::Color(100, 110, 130)); b.text.setFont(font);
        b.text.setString(btnLabels[i]); b.text.setCharacterSize(15);
        b.text.setFillColor(sf::Color::White); b.text.setPosition(935, 112 + i * 60);
        b.id = static_cast<int>(i); buttons.push_back(b);
    }

    const float roomSize = 160.0f;
    const float offsetX = 400.0f; 
    const float offsetY = 450.0f; 
    sf::Clock simClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                float mx = static_cast<float>(event.mouseButton.x);
                float my = static_cast<float>(event.mouseButton.y);
                
                bool buttonClicked = false;
                for (auto& btn : buttons) {
                    if (btn.rect.getGlobalBounds().contains(mx, my)) {
                        buttonClicked = true;
                        try {
                            switch (btn.id) {
                                case 0: { 
                                    Room newRoom("Camera_" + std::to_string(myHouse.getRoomCount()), Thermostat("Gen", 15.0 + (rand() % 15), 22.0));
                                    newRoom.addDevice(DeviceFactory::createDevice("Lumina"));
                                    newRoom.addDevice(DeviceFactory::createDevice("Calorifer"));
                                    newRoom.addDevice(DeviceFactory::createDevice("Aer Conditionat"));
                                    newRoom.addDevice(DeviceFactory::createDevice("Jaluzele"));
                                    myHouse.addRoom(newRoom); roomVisuals.push_back(getNewRoomPosition(roomVisuals));
                                    break;
                                }
                                case 1: myHouse.enterHouse(); break;
                                case 2: myHouse.leaveHouse(); break;
                                case 3: for(int i=0; i<myHouse.getRoomCount(); ++i) myHouse.getroom(i).regulateClimate(); break;
                                case 4: myHouse.setOutsideTemp(myHouse.getOutsideTemp() + 5.0); break;
                                case 5: myHouse.setOutsideTemp(myHouse.getOutsideTemp() - 5.0); break;
                                case 6: for(int i=0; i<myHouse.getRoomCount(); ++i) myHouse.getroom(i).changetemp(myHouse.getroom(i).getThermostat().getDesiredTemp() + 1.0); break;
                                case 7: for(int i=0; i<myHouse.getRoomCount(); ++i) myHouse.getroom(i).changetemp(myHouse.getroom(i).getThermostat().getDesiredTemp() - 1.0); break;
                                case 8: std::cout << myHouse << "\n"; break;
                                case 9: window.close(); break;
                            }
                        } catch (const SmartHomeException& e) {
                            Logger::getInstance().error(e.what());
                        } catch (...) {}
                    }
                }
                
                if (!buttonClicked) {
                    sf::Vector2f worldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), houseView);
                    for (int i = 0; i < myHouse.getRoomCount(); ++i) {
                        float rx = offsetX + roomVisuals[i].gridX * (roomSize + 15);
                        float ry = offsetY + roomVisuals[i].gridY * (roomSize + 15);
                        if (worldPos.x >= rx && worldPos.x <= rx + roomSize && worldPos.y >= ry && worldPos.y <= ry + roomSize) {
                            auto lights = filterDevicesByType<LightSource>(myHouse.getroom(i).getDevices());
                            for (auto& light : lights) {
                                if (light->getState()) {
                                    light->turnOff();
                                } else {
                                    light->setBrightness(100);
                                }
                            }
                        }
                    }
                }
            }
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) houseView.move(-25.f, 0.f);
                if (event.key.code == sf::Keyboard::Right) houseView.move(25.f, 0.f);
                if (event.key.code == sf::Keyboard::Up) houseView.move(0.f, -25.f);
                if (event.key.code == sf::Keyboard::Down) houseView.move(0.f, 25.f);
            }
        }

        if (simClock.getElapsedTime().asSeconds() >= 0.5f) {
            myHouse.simulationStep();
            simClock.restart();
        }

        window.clear(sf::Color(21, 23, 30)); 

        window.setView(houseView);
        for (int i = 0; i < myHouse.getRoomCount(); ++i) {
            float rx = offsetX + roomVisuals[i].gridX * (roomSize + 15);
            float ry = offsetY + roomVisuals[i].gridY * (roomSize + 15);
            bool hasLightOn = false;
            for (const auto& dev : myHouse.getroom(i).getDevices()) if (dev->getName() == "Lumina" && dev->getState()) hasLightOn = true;
            
            sf::RectangleShape roomShape(sf::Vector2f(roomSize, roomSize));
            roomShape.setPosition(rx, ry);
            roomShape.setFillColor(hasLightOn ? sf::Color(254, 240, 138) : sf::Color(55, 65, 81));
            roomShape.setOutlineThickness(3); roomShape.setOutlineColor(sf::Color(156, 163, 175));
            window.draw(roomShape);
            
            std::string runStatus = "INERTIE (spre afara)";
            
            auto heaters = filterDevicesByType<Heater>(myHouse.getroom(i).getDevices());
            auto acUnits = filterDevicesByType<AC>(myHouse.getroom(i).getDevices());

            if (hasLightOn) {
                runStatus = "STABIL";
                for (const auto& h : heaters) {
                    if (h->getState()) runStatus = "INCALZESTE ^";
                }
                for (const auto& ac : acUnits) {
                    if (ac->getState()) runStatus = "RACESTE v";
                }
            }

            sf::Text info; info.setFont(font); info.setCharacterSize(13); info.setFillColor(hasLightOn ? sf::Color::Black : sf::Color::White); info.setPosition(rx + 12, ry + 12);
            char metrics[128]; snprintf(metrics, sizeof(metrics), "%s\n\nStare: %s\nCurent: %.1f C\nTinta: %.1f C", myHouse.getroom(i).getName().c_str(), runStatus.c_str(), myHouse.getroom(i).getThermostat().getCurrentTemp(), myHouse.getroom(i).getThermostat().getDesiredTemp());
            info.setString(metrics); window.draw(info);
            
            int edgeSlot = 0; 
            for (const auto& dev : myHouse.getroom(i).getDevices()) {
                if (dev->getName() == "Lumina") continue;
                sf::RectangleShape deviceShape(sf::Vector2f(32, 14));
                if (dev->getName() == "Calorifer") deviceShape.setFillColor(dev->getState() ? sf::Color(239, 68, 68) : sf::Color(127, 29, 29));
                else if (dev->getName() == "Aer Conditionat") deviceShape.setFillColor(dev->getState() ? sf::Color(6, 182, 212) : sf::Color(30, 58, 138));
                else if (dev->getName() == "Jaluzele") deviceShape.setFillColor(dev->getState() ? sf::Color(245, 158, 11) : sf::Color(31, 41, 55));
                if (edgeSlot == 0) deviceShape.setPosition(rx + roomSize/2 - 16, ry);
                else if (edgeSlot == 1) { deviceShape.setSize(sf::Vector2f(14, 32)); deviceShape.setPosition(rx + roomSize - 14, ry + roomSize/2 - 16); }
                else if (edgeSlot == 2) deviceShape.setPosition(rx + roomSize/2 - 16, ry + roomSize - 14);
                else if (edgeSlot == 3) { deviceShape.setSize(sf::Vector2f(14, 32)); deviceShape.setPosition(rx, ry + roomSize/2 - 16); }
                window.draw(deviceShape);
                edgeSlot = (edgeSlot + 1) % 4;
            }
        }

        window.setView(window.getDefaultView());
        sf::RectangleShape topHeader(sf::Vector2f(1200, 70)); topHeader.setFillColor(sf::Color(32, 38, 51)); window.draw(topHeader);
        sf::Text globalText; globalText.setFont(font); globalText.setCharacterSize(18); globalText.setFillColor(sf::Color(52, 211, 153)); globalText.setPosition(30, 22);
        globalText.setString("Temp Exterior: " + std::to_string(static_cast<int>(myHouse.getOutsideTemp())) + " C");
        window.draw(globalText);
        sf::RectangleShape panel(sf::Vector2f(300, 800)); panel.setPosition(900, 0); panel.setFillColor(sf::Color(28, 33, 46)); window.draw(panel);
        for (auto& btn : buttons) { window.draw(btn.rect); window.draw(btn.text); }
        
        window.display();
    }
    return 0;
}