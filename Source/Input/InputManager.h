#ifndef EX2_INPUTPARSER_H
#define EX2_INPUTPARSER_H


#include "../Driver/Driver.h"
#include "../Cab/TaxiCenter.h"
#include "../Driver/Client.h"
#include <sstream>

/**
 * Read and parse input drom the user.
 */
class InputManager {
public:
    static int readNumber();

    static bool readObstacles(int numObstacles, list<Point> *obstacles,
                              int width, int height);

    static CityMap *readCityMap();

    static Client *readClient(string ip_addr, uint16_t port);

    static Cab *readCab();

    static Ride *readRide(CityMap *cityMap);

    static MaritalStatus parseStatus(char chr);

    static Manufacturer parseManufacturer(string str);

    static Color parseColor(string str);

    static string readLine();

private:
    static string parseWord(string &input);

    static int countFlag(string str, char flag);
};


#endif //EX2_INPUTPARSER_H
