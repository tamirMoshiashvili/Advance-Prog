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
        s >> firstVal >> comma >> secondVal;
        if (s.fail() || !s.eof() || comma != ',' || firstVal < 0
            || firstVal >= width || secondVal < 0 || secondVal >= height) {
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
        s >> width >> height;
        if (s.fail() || !s.eof() || width < 1 || height < 1) {
            // Invalid map-size.
            BOOST_LOG_TRIVIAL(debug) << "wrong map sizes";
            cin.clear();
        } else {
            // Read number of obstacles that will be.
            getline(cin, in);
            stringstream s2(in);
            s2 >> numObstacles;
            if (s2.fail() || !s2.eof() || numObstacles < 0) {
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
 * Check if all the chars inside the array are equal to the given flag.
 * @param flags_arr array of chars.
 * @param size size of the array.
 * @param flag char.
 * @return true if the array contains only the given flag in all of its cells.
 */
bool InputManager::checkIfFlags(char *flags_arr, int size, char flag) {
    for (int i = 0; i < size; ++i) {
        if (flags_arr[i] != flag) {
            // Invalid char instead of comma.
            BOOST_LOG_TRIVIAL(debug) << "Invalid char instead of: '" << flag
                                     << "' .";
            return false;
        }
    }
    return true;
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
    char status_chr, comma[4];
    // Input.
    s >> id >> comma[0] >> age >> comma[1] >> status_chr
      >> comma[2] >> experience >> comma[3] >> cabId;
    // Check for valid commas.
    if (!checkIfFlags(comma, 4, ',')) {
        return NULL;
    }
    m_status = InputManager::parseStatus(status_chr);
    if (s.fail() || id < 0 || age < 0 || m_status > 3
        || experience < 0 || cabId < 0) {
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
    string in;
    getline(cin, in);
    if (countFlag(in, ',') != 3) {
        // Invalid input.
        BOOST_LOG_TRIVIAL(debug) << "Invalid number of commas";
        return NULL;
    }
    stringstream s(in);
    int id, type;
    Manufacturer manufacturer;
    Color color;
    char manufacturer_chr, color_chr, comma[3];
    // Input.
    s >> id >> comma[0] >> type >> comma[1]
      >> manufacturer_chr >> comma[2] >> color_chr;
    BOOST_LOG_TRIVIAL(debug) << comma[0] << comma[1] << comma[2];
    if (!checkIfFlags(comma, 3, ',')) {
        BOOST_LOG_TRIVIAL(debug) << "Not a comma";
        return NULL;
    }
    manufacturer = InputManager::parseManufacturer(manufacturer_chr);
    color = InputManager::parseColor(color_chr);
    BOOST_LOG_TRIVIAL(debug) << id << " " << type <<" "<< manufacturer << " "
                             << color;
    if (s.fail() ||  id < 0 || type < 1
        || type > 2 || manufacturer == DEFAULT_MANUFACTURER ||
        color == DEFAULT_COLOR) {
        // Invalid input.
        BOOST_LOG_TRIVIAL(debug) << "Invalid arguments of vars";
        return NULL;
    }
    //Create a cab of type according to the input.
    Cab *cab = NULL;
    //TODO: add cab-factory.
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
    string in;
    getline(cin, in);
    if (countFlag(in, ',') != 7) {
        // Invalid input.
        return NULL;
    }
    stringstream s(in);
    int id, xStart, yStart, xEnd, yEnd, numPassengers, time;
    double tariff;
    char comma[7];
    // Input.
    s >> id >> comma[0] >> xStart >> comma[1] >> yStart
      >> comma[2] >> xEnd >> comma[3] >> yEnd >> comma[4] >> numPassengers
      >> comma[5] >> tariff >> comma[6] >> time;

    if (!checkIfFlags(comma, 7, ',')) {
        BOOST_LOG_TRIVIAL(debug) << "Not a comma";
        return NULL;
    }
    int mapWidth = cityMap->getWigth(), mapHeight = cityMap->getHeight();

    if (s.fail() || !s.eof() || id < 0 || xStart < 0 || xStart >= mapWidth ||
        yStart < 0 || yStart >= mapHeight || xEnd < 0 || xEnd >= mapWidth ||
        yEnd < 0 || yEnd >= mapHeight || numPassengers < 0 || tariff < 0 ||
        time < 1 || cityMap->getBlock(xStart, yStart)->checkIfVisited() ||
        cityMap->getBlock(xEnd, yEnd)->checkIfVisited()) {
        // Invalid input.
        BOOST_LOG_TRIVIAL(debug) << "Invalid arguments of vars";
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
Manufacturer InputManager::parseManufacturer(char chr) {
    char arr[1] = {chr};
    string str(arr);
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
Color InputManager::parseColor(char chr) {
    char arr[1] = {chr};
    string str(arr);
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
