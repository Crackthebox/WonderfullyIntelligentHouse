#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "source.h"

int main() {
    
    Thermostat termSmart("Asm", 24.5, 22.0); 
    Thermostat copieTerm = termSmart;                
    
    Thermostat termSimplu("Generic", 18.0, 20.0);
    copieTerm = termSimplu;                          

    Room living("Living Room", Thermostat("Samsung", 25.0, 22.0)); 
    Room dormitor("Dormitor", Thermostat("Apple", 18.0, 22.0));

    House myHouse("Strada Magnifica");
    myHouse.addRoom(living);
    myHouse.addRoom(dormitor);

    std::cout << myHouse;

    std::cout<<"\nAm intrat in casa\n";
    myHouse.enterHouse();

    std::cout << myHouse;

    std::cout<<"\nAm modificat conditiile din camere\n";
    myHouse.changetemp(27,0);
    myHouse.changetemp(15,1);
    myHouse.getroom(0).regulateClimate();
    myHouse.getroom(1).regulateClimate();
    myHouse.printRoom(0);
    myHouse.printRoom(1);
    std::cout<<"\nAm iesit din casa\n";
    myHouse.leaveHouse();

    std::cout << myHouse;
    
    
    return 0;
}