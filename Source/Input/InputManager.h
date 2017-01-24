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

    static std::list<Point> readObstacles(int numObstacles);

    static CityMap *readCityMap();

    static Client *readClient(string ip_addr, uint16_t port);

    static Cab *readCab();

    static Ride *readRide(CityMap *cityMap);

    static MaritalStatus parseStatus(string str);

    static Manufacturer parseManufacturer(string str);

    static Color parseColor(string str);

    static string readLine();

private:
    static string parseWord(string &input);

    static int countComma(string str);
};


#endif //EX2_INPUTPARSER_H
