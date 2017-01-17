#include "Point.h"
#include <boost/lexical_cast.hpp>

/**
 * Constructor.
 * @param x value on the x axis.
 * @param y value on the y axis.
 * @return Point object.
 */
Point::Point(int xVal, int yVal) : x(xVal), y(yVal) {

}

/**
 * Default constructor.
 * @return Point object.
 */
Point::Point() : x(0), y(0) {

}

/**
 * Destructor.
 */
Point::~Point() {

}

/**
 * Get the x value of the point.
 * @return number.
 */
int Point::getX() {
    return x;
}

/**
 * Get the y value of the point.
 * @return number.
 */
int Point::getY() {
    return y;
}

/**
 * Check if the current point is equal to the given one.
 * @param other point.
 * @return true if the points are identical, false otherwise.
 */
bool Point::operator==(Point other) {
    // Check if the x and y values are equal
    return x == other.getX() && y == other.getY();
}

/**
 * Check if the current point is not equal to the given one.
 * @param other point.
 * @return true if the points are not identical, false otherwise.
 */
bool Point::operator!=(Point other) {
    return x != other.getX() || y != other.getY();
}

/**
 * Print the point in the form of ( x , y ).
 */
ostream &operator<<(ostream &os, const Point &point) {
    return os << "(" << point.x << "," << point.y << ")" << endl;
}

/**
 * Assign this point according to other point values.
 * @param point point object.
 */
void Point::operator=(const Point &point) {
    x = point.x;
    y = point.y;
}

/**
 * Get a string-representation of the point.
 * @return string in format "x,y".
 */
string Point::getRepresentation() {
    stringstream out;
    out << x << "," << y;
    return out.str();
}

Point Point::strToPoint(string str) {
    unsigned long j = str.find(",");
    int x = atoi(str.substr(0, j).c_str());
    int y = atoi(str.substr(j + 1, str.length()).c_str());
    return Point(x, y);
}

