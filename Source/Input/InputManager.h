#ifndef EX2_INPUTPARSER_H
#define EX2_INPUTPARSER_H


#include "../Driver/Driver.h"
#include "../Cab/TaxiCenter.h"
#include "../Driver/Client.h"

/**
 * Read and parse input drom the user.
 */
class InputManager {
public:
    static int readNumber();

    static std::list<Point> readObstacles();

    static CityMap *readCityMap();

    static Client *readClient(string ip_addr, uint16_t port);

    static Cab *readCab(TaxiCenter *taxiCenter);

    static Ride *readRide();

    static MaritalStatus parseStatus(string str);

    static Manufacturer parseManufacturer(string str);

    static Color parseColor(string str);

private:
    static string parseWord(string &input);

    static string readLine();
};


#endif //EX2_INPUTPARSER_H
