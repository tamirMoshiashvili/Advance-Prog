#ifndef EX2_TRIPINFO_H
#define EX2_TRIPINFO_H


#include <list>
#include "../Basic/Point.h"
#include "../Basic/Recognizable.h"
#include "../Basic/WayPasser.h"
#include "Passenger.h"
#include <boost/serialization/list.hpp>

/**
 * Represents a ride that goes through two points.
 */
class Ride : public Recognizable, public WayPasser {
private:
    Point startPoint, endPoint;
    int numPassengers;
    double tariff;
    list<Passenger *> passengers;
    int startTime;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Recognizable>(*this);
        ar & boost::serialization::base_object<WayPasser>(*this);
        ar & startPoint;
        ar & endPoint;
        ar & numPassengers;
        ar & tariff;
        ar & passengers;
        ar & startTime;
    }

public:
    Ride(int idNum, Point start, Point end,
         int numOfPassengers, double tariffVal, int time);

    Ride();

    ~Ride();

    double getTariff();

    list<Passenger *> getPassengers();

    Point getSourcePoint();

    Point getDestinationPoint();

    int getStartTime();

};


#endif //EX2_TRIPINFO_H
