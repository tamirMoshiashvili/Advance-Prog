#ifndef EX2_PASSENGER_H
#define EX2_PASSENGER_H


#include "../Basic/Point.h"

/**
 * Represents a passenger that start in some point,
 * and need to go to another point.
 */
class Passenger {
private:
    Point sourcePoint, destinationPoint;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & sourcePoint;
        ar & destinationPoint;
    }

public:
    Passenger(Point source, Point destination);

    Passenger();

    ~Passenger();

    int getSatisfaction();
};


#endif //EX2_PASSENGER_H
