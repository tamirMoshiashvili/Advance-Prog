#include "InputManager.h"
#include "../Cab/Vehicle/StandardCab.h"
#include "../Cab/Vehicle/LuxuryCab.h"
#include <stdlib.h>
#include <boost/log/trivial.hpp>

using namespace std;

/**
 * Read number from the user,
 * @return number.
 */
int InputManager::readNumber() {
    int singleSize;
    cin >> singleSize;
    return singleSize;
}

/**
 * Read from the user list of points, where the obstacles will be.
 * @return list of points.
 */
list <Point> InputManager::readObstacles() {
    list <Point> obstacles = list<Point>();
    // Read number of obstacles that will be.
    int numObstacles;
    cin >> numObstacles;
    string obstacleData;
    string first, second;
    size_t separator;
    int firstVal, secondVal;
    for (int i = 0; i < numObstacles; ++i) {
        // Get one obstacle, of form "number,number" .
        cin >> obstacleData;
        // Find the index of the separator.
        separator = obstacleData.find(",");
        // Calculate first value of the point.
        first = obstacleData.substr(0, separator);
        firstVal = atoi(first.c_str());
        // Calculate second value of the point.
        second = obstacleData.substr(separator + 1, obstacleData.length() - 1);
        secondVal = atoi(second.c_str());
        // Add the point to the list of obstacles.
        obstacles.push_back(Point(firstVal, secondVal));
    }
    return obstacles;
}

/**
 * Produce a map according to the user input.
 * Note: the object is in the heap.
 * @return pointer to city-map.
 */
CityMap *InputManager::readCityMap() {
    int width = readNumber(), height = readNumber();
    list <Point> obstacles = readObstacles();
    // Create the map.
    CityMap *map = new CityMap(width, height);
    int x, y;
    // Iterate through the obstacles-list and add each to the map.
    for (std::list<Point>::iterator it = obstacles.begin();
         it != obstacles.end(); it++) {
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
 * @return string.
 */
string InputManager::readLine() {
    string str;
    getline(cin, str);
    return str;
}

/**
 * Count the number of commas are in the given string.
 * @param str string.
 * @return number of commas.
 */
int InputManager::countComma(string str) {
    int numComma = 0;
    for (unsigned long i = 0; i < str.length(); ++i) {
        if (str.at(i) == ',') {
            ++numComma;
        }
    }
    return numComma;
}

/**
 * Create new driver according to user's input.
 * @param ip_addr ip address.
 * @param port port number.
 * @return pointer to client.
 */
Client *InputManager::readClient(string ip_addr, uint16_t port) {
    // Read a string from the user.
    string str = readLine(), subStr;
    if (countComma(str) != 4) {
        // Invalid input.
        return NULL;
    }
    // Parse the input.
    int id = atoi(parseWord(str).c_str());
    int age = atoi(parseWord(str).c_str());
    MaritalStatus maritalStatus;
    subStr = parseWord(str);
    maritalStatus = InputManager::parseStatus(subStr);
    int experience = atoi(parseWord(str).c_str());
    int cabID = atoi(str.c_str());
    if (id < 0 || age < 0 || maritalStatus < 0 || maritalStatus > 3 ||
        experience < 0 || cabID < 0) {
        // Invalid input.
        return NULL;
    }
    return new Client(id, age, maritalStatus, experience, cabID, ip_addr, port);
}

/**
 * Create new cab according to user's input.
 * Note: the object is in the heap.
 * @param taxiCenter pointer to taxi-center.
 * @return pointer to cab.
 */
Cab *InputManager::readCab(TaxiCenter *taxiCenter) {
    // Read a string from the user.
    string str = readLine(), subStr;
    // Parse the input.
    int id = atoi(parseWord(str).c_str());
    int type = atoi(parseWord(str).c_str());
    subStr = parseWord(str);
    Manufacturer manufacturer = InputManager::parseManufacturer(subStr);
    subStr = parseWord(str);
    Color color = InputManager::parseColor(subStr);
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
 * @return pointer to ride.
 */
Ride *InputManager::readRide() {
    // Read a string from the user.
    string str = readLine();
    // Parse the input.
    int id = atoi(parseWord(str).c_str());
    int xStart = atoi(parseWord(str).c_str());
    int yStart = atoi(parseWord(str).c_str());
    int xEnd = atoi(parseWord(str).c_str());
    int yEnd = atoi(parseWord(str).c_str());
    int numPassengers = atoi(parseWord(str).c_str());
    double tariff = strtod(parseWord(str).c_str(), NULL);
    int time = atoi(str.c_str());
    return new Ride(id, Point(xStart, yStart), Point(xEnd, yEnd),
                    numPassengers, tariff, time);
}

/**
 * Parse a string into marital-status.
 * @param str string that represents marital-status.
 * @return marital-status object.
 */
MaritalStatus InputManager::parseStatus(string str) {
    MaritalStatus maritalStatus = SINGLE;
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
    Manufacturer manufacturer = HONDA;
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
    Color color = RED;
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
