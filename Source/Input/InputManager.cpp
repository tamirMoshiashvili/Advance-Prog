#include "InputManager.h"
#include "../Cab/Vehicle/StandardCab.h"
#include "../Cab/Vehicle/LuxuryCab.h"
#include <stdlib.h>
#include <boost/log/trivial.hpp>

using namespace std;

/**
 * Read from the user list of points, where the obstacles will be.
 * @param numObstacles number of obstacles to get.
 * @param obstacles
 * @param width width of the map.
 * @param height height of the map.
 * @return list of points.
 */
bool InputManager::readObstacles(int numObstacles, list <Point> *obstacles,
                                 int width, int height) {
    int firstVal, secondVal;
    char comma;
    for (int i = 0; i < numObstacles; ++i) {
        string in;
        getline(cin, in);
        if (countFlag(in, ',') != 1) {
            // Invalid number of commas.
            BOOST_LOG_TRIVIAL(debug) << "Invalid number of commas";
            return false;
        }
        stringstream s(in);
        // Get one obstacle, of form "number,number" .
        char dummy = 0;
        s >> firstVal >> comma >> secondVal >> dummy;
        if (!cin.good() || comma != ',' || firstVal < 0
            || firstVal >= width || secondVal < 0
            || secondVal >= height || dummy != 0) {
            // Invalid arg for obstacle.
            BOOST_LOG_TRIVIAL(debug) << "Invalid arg for obstacle";
            return false;
        }
        // Add the point to the list of obstacles.
        obstacles->push_back(Point(firstVal, secondVal));
    }
    return true;
}

/**
 * Produce a map according to the user input.
 * Note: the object is in the heap.
 * @return pointer to city-map.
 */
CityMap *InputManager::readCityMap() {
    int width, height, numObstacles;
    list <Point> obstacles;
    string in;
    while (true) {
        getline(cin, in);
        stringstream s(in);
        char dummy = 0;
        s >> width >> height >> dummy;
        if (!cin.good() || width < 1 || height < 1 || dummy != 0) {
            // Invalid map-size.
            BOOST_LOG_TRIVIAL(debug) << "wrong map sizes";
            cin.clear();
        } else {
            // Read number of obstacles that will be.
            dummy = 0;
            getline(cin, in);
            stringstream s2(in);
            s2 >> numObstacles >> dummy;
            if (!cin.good() || numObstacles < 0 || dummy != 0) {
                // Invalid number of obstacles.
                BOOST_LOG_TRIVIAL(debug) << "wrong number of obstacles";
                cin.clear();
            } else {
                if (!readObstacles(numObstacles, &obstacles, width, height)) {
                    // Invalid obstacle.
                    cin.clear();
                } else {
                    // Valid input.
                    break;
                }
            }
        }
    }
    BOOST_LOG_TRIVIAL(debug) << "width: " << width
                             << ", height: " << height
                             << ", num-obstacles: " << numObstacles;
    int x, y;
    // Create the map.
    CityMap *map = new CityMap(width, height);
    // Add all the obstacles to the map.
    for (std::list<Point>::iterator it = obstacles.begin();
         it != obstacles.end(); ++it) {
        x = (*it).getX();
        y = (*it).getY();
        map->addObstacle(x, y);
    }
    return map;
}

/**
 * Gets a string and parse the substring till the ',' character.
 * Then erase that part from the original string.
 * @param input reference to string.
 * @return a substring of the input.
 */
string InputManager::parseWord(string &input) {
    unsigned long int i = input.find(',');
    string subStr = input.substr(0, i);
    input.erase(0, i + 1);
    return subStr;
}

/**
 * Read full line from the user.
 * @return string string.
 */
string InputManager::readLine() {
    string str;
    getline(cin, str);
    return str;
}

/**
 * Count the number of flags in the given string.
 * @param str string.
 * @param flag char to find.
 * @return number of flags in the string.
 */
int InputManager::countFlag(string str, char flag) {
    int num = 0;
    for (unsigned long i = 0; i < str.length(); ++i) {
        if (str.at(i) == flag) {
            ++num;
        }
    }
    return num;
}

/**
 * Create new driver according to user's input.
 * @param ip_addr ip address.
 * @param port port number.
 * @return pointer to client.
 */
Client *InputManager::readClient(string ip_addr, uint16_t port) {
    string in;
    getline(cin, in);
    if (countFlag(in, ',') != 4) {
        // Invalid number of commas.
        BOOST_LOG_TRIVIAL(debug) << "Invalid number of commas";
        return NULL;
    }
    stringstream s(in);
    int id, age, experience, cabId;
    MaritalStatus m_status;
    char status_chr, comma[4], dummy = 0;
    // Input.
    s >> id >> comma[0] >> age >> comma[1] >> status_chr
      >> comma[2] >> experience >> comma[3] >> cabId >> dummy;
    // Check for valid commas.
    for (int i = 0; i < 4; ++i) {
        if (comma[i] != ',') {
            // Invalid char instead of comma.
            BOOST_LOG_TRIVIAL(debug) << "Invalid char instead of comma";
            return NULL;
        }
    }
    m_status = InputManager::parseStatus(status_chr);
    if (!cin.good() || id < 0 || age < 0 || m_status > 3
        || experience < 0 || cabId < 0 || dummy != 0) {
        // Invalid input.
        BOOST_LOG_TRIVIAL(debug) << "Invalid arguments of vars";
        return NULL;
    }
    // Valid input.
    return new Client(id, age, m_status, experience, cabId, ip_addr, port);
}

/**
 * Create new cab according to user's input.
 * Note: the object is in the heap.
 * @param taxiCenter pointer to taxi-center.
 * @return pointer to cab.
 */
Cab *InputManager::readCab() {
    // Read a string from the user.
    string str = readLine(), subStr;
    if (countFlag(str, ',') != 3) {
        // Invalid input.
        return NULL;
    }
    // Parse the input.
    int id = atoi(parseWord(str).c_str());
    int type = atoi(parseWord(str).c_str());
    subStr = parseWord(str);
    Manufacturer manufacturer = InputManager::parseManufacturer(subStr);
    subStr = parseWord(str);
    Color color = InputManager::parseColor(subStr);
    if (id < 0 || type < 1 || type > 2 || manufacturer > 3 || color > 4) {
        return NULL;
    }
    // Create a cab of type according to the input.
    Cab *cab = NULL;
    if (type == 1) {
        cab = new StandardCab(id, manufacturer, color, 1);
    } else if (type == 2) {
        cab = new LuxuryCab(id, manufacturer, color, 2);
    }
    return cab;
}

/**
 * Create new ride according to user's input.
 * Note: the object is in the heap.
 * @param cityMap city-map;
 * @return pointer to ride.
 */
Ride *InputManager::readRide(CityMap *cityMap) {
    // Read a string from the user.
    string str = readLine();
    if (countFlag(str, ',') != 7) {
        // Invalid input.
        return NULL;
    }
    // Parse the input.
    int id = atoi(parseWord(str).c_str());
    int xStart = atoi(parseWord(str).c_str());
    int yStart = atoi(parseWord(str).c_str());
    int xEnd = atoi(parseWord(str).c_str());
    int yEnd = atoi(parseWord(str).c_str());
    int numPassengers = atoi(parseWord(str).c_str());
    double tariff = strtod(parseWord(str).c_str(), NULL);
    int time = atoi(str.c_str());
    int mapWidth = cityMap->getWigth(), mapHeight = cityMap->getHeight();
    if (id < 0 || xStart < 0 || xStart >= mapWidth || yStart < 0 ||
        yStart >= mapHeight || xEnd < 0 || xEnd >= mapWidth || yEnd < 0 ||
        yEnd >= mapHeight || numPassengers < 0 || tariff < 0 || time < 1 ||
        cityMap->getBlock(xStart, yStart)->checkIfVisited() ||
        cityMap->getBlock(xEnd, yEnd)->checkIfVisited()) {
        return NULL;
    }
    return new Ride(id, Point(xStart, yStart), Point(xEnd, yEnd),
                    numPassengers, tariff, time);
}

/**
 * Parse a string into marital-status.
 * @param chr char that represents marital-status.
 * @return marital-status object.
 */
MaritalStatus InputManager::parseStatus(char chr) {
    char arr[1] = {chr};
    string str(arr);
    MaritalStatus maritalStatus = DEFAULT_STATUS;
    if (!str.compare("S")) {
        maritalStatus = SINGLE;
    } else if (!str.compare("M")) {
        maritalStatus = MARRIED;
    } else if (!str.compare("D")) {
        maritalStatus = DIVORCED;
    } else if (!str.compare("W")) {
        maritalStatus = WIDOWED;
    }
    return maritalStatus;
}

/**
 * Parse a string into manufacturer.
 * @param str string that represents manufacturer.
 * @return manufacturer object.
 */
Manufacturer InputManager::parseManufacturer(string str) {
    Manufacturer manufacturer = DEFAULT_MANUFACTURER;
    if (!str.compare("H")) {
        manufacturer = HONDA;
    } else if (!str.compare("S")) {
        manufacturer = SUBARO;
    } else if (!str.compare("T")) {
        manufacturer = TESLA;
    } else if (!str.compare("F")) {
        manufacturer = FIAT;
    }
    return manufacturer;
}

/**
 * Parse a string into color.
 * @param str string that represents color.
 * @return color object.
 */
Color InputManager::parseColor(string str) {
    Color color = DEFAULT_COLOR;
    if (!str.compare("R")) {
        color = RED;
    } else if (!str.compare("B")) {
        color = BLUE;
    } else if (!str.compare("G")) {
        color = GREEN;
    } else if (!str.compare("P")) {
        color = PINK;
    } else if (!str.compare("W")) {
        color = WHITE;
    }
    return color;
}
