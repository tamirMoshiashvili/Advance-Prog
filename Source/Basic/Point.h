#ifndef EX2_POINT_H
#define EX2_POINT_H

#include <iostream>
#include <boost/serialization/access.hpp>

using namespace std;

/**
 * Represents a coordinates on the x and y axes.
 */
class Point {
private:
    int x, y;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & x;
        ar & y;
    }

public:
    Point(int xVal, int yVal);

    Point();

    ~Point();

    int getX();

    int getY();

    bool operator==(Point other);

    bool operator!=(Point other);

    friend ostream &operator<<(ostream &os, const Point &point);

    void operator=(const Point &point);

    string getRepresentation();
};


#endif //EX2_POINT_H
