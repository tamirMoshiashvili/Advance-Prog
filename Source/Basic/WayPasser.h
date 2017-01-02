#ifndef EX2_WAYPASSER_H
#define EX2_WAYPASSER_H

#include <boost/serialization/access.hpp>

/**
 * Represents an object that can pass a way.
 */
class WayPasser {
protected:
    int numKilometersPassed;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & numKilometersPassed;
    }

public:
    WayPasser();

    virtual ~WayPasser();

    void addKilometersPassed(int num);

    int getNumKilometersPassed();

};


#endif //EX2_WAYPASSER_H
