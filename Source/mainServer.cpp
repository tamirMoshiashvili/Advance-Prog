#include <iostream>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include "Cab/TaxiCenter.h"
#include "Control/MainFlow.h"
#include "Input/InputManager.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;


int main(int argc, char **argv) {
    string port = argv[1];
    // Create objects.
    CityMap *map = InputManager::readCityMap();
    TaxiCenter *taxiCenter = new TaxiCenter(map);
    MainFlow mainFlow(taxiCenter);
    mainFlow.operateTaxiCenter((uint16_t) atoi(port.c_str()), map);
    return 0;
}
