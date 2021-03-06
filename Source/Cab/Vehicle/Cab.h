#ifndef EX2_CAB_H
#define EX2_CAB_H

#include "../../Basic/Point.h"
#include "../../Ride/Navigation/PathCalculator.h"
#include "../../Basic/Recognizable.h"
#include "../../Basic/WayPasser.h"
#include "../../Ride/Navigation/Navigation.h"

/**
 * Represents a manufacturer of cars.
 */
enum Manufacturer {
    HONDA, SUBARO, TESLA, FIAT, DEFAULT_MANUFACTURER
};

/**
 * Represents a color.
 */
enum Color {
    RED, BLUE, GREEN, PINK, WHITE, DEFAULT_COLOR
};

/**
 * Represents a cab that holds a data about itself.
 */
class Cab : public Recognizable, public WayPasser {
private:
    Manufacturer manufacturer;
    Color color;
    double tariff;
    Point location;
    Navigation *navigation;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Recognizable>(*this);
        ar & boost::serialization::base_object<WayPasser>(*this);
        ar & manufacturer;
        ar & color;
        ar & tariff;
        ar & location;
        ar & navigation;
    }

public:
    Cab(int idNum, Manufacturer manufacturer1,
        Color color1, double tariffVal);

    Cab();

    virtual ~Cab();

    Point getLocation();

    void setLocation(Point point);

    double getTariff();

    void setNavigation(Navigation *navigation1);

    virtual int moveOneStep();

    virtual int getSizeOfStep()=0;

    bool isArrivedToDestination();

    void resetNavigation();

};


#endif //EX2_CAB_H
