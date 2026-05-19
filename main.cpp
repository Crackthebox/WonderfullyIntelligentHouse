#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include "source.h"
#include "exceptions.h"

void showMenu(const std::string& currentHouseAddress) {
    std::cout << "\n=== SMART HOME DASHBOARD ===\n";
    std::cout << "Casa Curenta: " << currentHouseAddress << "\n";
    std::cout << "1. Afiseaza starea casei\n";
    std::cout << "2. Modifica temperatura dorita intr-o camera\n";
    std::cout << "3. Intra in casa (Comfort)\n";
    std::cout << "4. Iesi din casa (Eco)\n";
    std::cout << "5. Regleaza automat clima\n";
    std::cout << "6. Seteaza manual lumina (Downcast)\n";
    std::cout << "7. Adauga o casa noua\n";
    std::cout << "8. Schimba casa curenta\n";
    std::cout << "9. Adauga o camera noua\n";
    std::cout << "10. Adauga un dispozitiv\n";
    std::cout << "11. Total dispozitive (Functie Static)\n";
    std::cout << "0. Iesire\n";
    std::cout << "Alegerea ta: ";
}

int main() {
    std::vector<House> myHouses;
    int currentHouseIdx = 0;

    std::ifstream fin("config_casa.txt");
    if (fin.is_open()) {
        int numHouses;
        if (fin >> numHouses) {
            for (int h = 0; h < numHouses; ++h) {
                fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::string address;
                std::getline(fin, address);
                House tempHouse(address);

                int numRooms;
                fin >> numRooms;
                for (int i = 0; i < numRooms; ++i) {
                    std::string rName;
                    double cTemp, dTemp;
                    fin >> rName >> cTemp >> dTemp;
                    
                    Room r(rName, Thermostat("Generic", cTemp, dTemp));
                    r.addDevice(std::make_shared<LightSource>());
                    r.addDevice(std::make_shared<Heater>());
                    if (i == 0) {
                        r.addDevice(std::make_shared<AC>());
                        r.addDevice(std::make_shared<SmartBlinds>());
                    }
                    tempHouse.addRoom(r);
                }
                myHouses.push_back(tempHouse);
            }
        }
        fin.close();
    } else {
        std::cerr << "[WARNING] fisierul config_casa.txt nu a fost gasit. Se incarca o casa goala.\n";
        myHouses.push_back(House("Casa Generica"));
    }

    bool isRunning = true;
    while (isRunning) {
        showMenu(myHouses[currentHouseIdx].getAddress());
        int choice;
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Introdu un numar valid!\n";
            continue; 
        }

        try {
            switch (choice) {
                case 1:
                    std::cout << myHouses[currentHouseIdx];
                    break;
                case 2: {
                    int rIdx; double newTemp;
                    std::cout << "Index camera: "; std::cin >> rIdx;
                    std::cout << "Temperatura dorita: "; std::cin >> newTemp;
                    myHouses[currentHouseIdx].changetemp(newTemp, rIdx);
                    std::cout << "Temperatura setata!\n";
                    break;
                }
                case 3:
                    myHouses[currentHouseIdx].enterHouse();
                    std::cout << "Mod Comfort activat.\n";
                    break;
                case 4:
                    myHouses[currentHouseIdx].leaveHouse();
                    std::cout << "Mod Eco activat.\n";
                    break;
                case 5: {
                    int rIdx;
                    std::cout << "Index camera: "; std::cin >> rIdx;
                    myHouses[currentHouseIdx].getroom(rIdx).regulateClimate();
                    std::cout << "Clima reglata!\n";
                    break;
                }
                case 6: {
                    int rIdx, bright;
                    std::cout << "Index camera: "; std::cin >> rIdx;
                    std::cout << "Luminozitate (0-100): "; std::cin >> bright;
                    myHouses[currentHouseIdx].getroom(rIdx).forceLightsBrightness(bright);
                    std::cout << "Lumina setata.\n";
                    break;
                }
                case 7: {
                    std::cin.ignore();
                    std::string addr;
                    std::cout << "Adresa casei noi: ";
                    std::getline(std::cin, addr);
                    myHouses.push_back(House(addr));
                    std::cout << "Casa adaugata cu succes!\n";
                    break;
                }
                case 8: {
                    int idx;
                    std::cout << "Alege indexul casei (0 - " << myHouses.size() - 1 << "): ";
                    std::cin >> idx;
                    if (idx < 0 || idx >= static_cast<int>(myHouses.size())) throw HouseNotFoundException(idx);
                    currentHouseIdx = idx;
                    std::cout << "Casa schimbata.\n";
                    break;
                }
                case 9: {
                    std::string rName; double ct, dt;
                    std::cout << "Nume camera (fara spatii): "; std::cin >> rName;
                    std::cout << "Temp curenta: "; std::cin >> ct;
                    std::cout << "Temp dorita: "; std::cin >> dt;
                    myHouses[currentHouseIdx].addRoom(Room(rName, Thermostat("Generic", ct, dt)));
                    std::cout << "Camera adaugata!\n";
                    break;
                }
                case 10: {
                    int rIdx, devType;
                    std::cout << "Index camera: "; std::cin >> rIdx;
                    myHouses[currentHouseIdx].getroom(rIdx); 
                    std::cout << "Tip dispozitiv (1-Lumina, 2-Calorifer, 3-AC, 4-Jaluzele): ";
                    std::cin >> devType;

                    std::shared_ptr<SmartDevice> nou;
                    if (devType == 1) nou = std::make_shared<LightSource>();
                    else if (devType == 2) nou = std::make_shared<Heater>();
                    else if (devType == 3) nou = std::make_shared<AC>();
                    else if (devType == 4) nou = std::make_shared<SmartBlinds>();
                    else throw SmartHomeException("Tip dispozitiv invalid!");

                    myHouses[currentHouseIdx].getroom(rIdx).addDevice(nou);
                    std::cout << "Dispozitiv adaugat cu succes!\n";
                    break;
                }
                case 11:
                    std::cout << "\n>>> Sistemul gestioneaza un total de " 
                              << SmartDevice::getTotalDevices() << " dispozitive smart in toate casele. <<<\n";
                    break;
                case 0:
                    isRunning = false;
                    break;
                default:
                    std::cout << "Optiune invalida!\n";
            }
        } 
        catch (const SmartHomeException& e) {
            std::cerr << "\n[EROARE CUSTOM]: " << e.what() << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "\n[EROARE SISTEM]: " << e.what() << "\n";
        }
    }
    
    return 0;
}