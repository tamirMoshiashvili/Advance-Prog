#ifndef EX2_CAB_H
#define EX2_CAB_H

#include "../../Basic/Point.h"
#include "../../Ride/Navigation/Navigation.h"
#include "../../Basic/Recognizable.h"
#include "../../Basic/WayPasser.h"
#include "../../Map/LocationDetector.h"

/**
 * Represents a manufacturer of cars.
 */
enum Manufacturer {
    HONDA, SUBARO, TESLA, FIAT
};

/**
 * Represents a color.
 */
enum Color {
    RED, BLUE, GREEN, PINK, WHITE
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
    LocationDetector *detector;

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
        ar & detector;
    }

public:
    Cab(int idNum, Manufacturer manufacturer1,
        Color color1, double tariffVal, LocationDetector *locationDetector);

    Cab();

    virtual ~Cab();

    Point getLocation();

    void setLocation(Point point);

    Color getColor();

    void setColor(Color color1);

    double getTariff();

    Manufacturer getManufacturer();

    void setNavigation(Navigation *navigation1);

    virtual int moveOneStep();

    virtual int getSizeOfStep()=0;

    bool isArrivedToDestination();

    void resetNavigation();

    LocationDetector *getLocationDetector();
};


#endif //EX2_CAB_H
