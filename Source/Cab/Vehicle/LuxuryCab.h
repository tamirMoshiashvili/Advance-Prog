#ifndef EX2_LUXURYCAB_H
#define EX2_LUXURYCAB_H


#include "Cab.h"

/**
 * Represents a luxury cab.
 */
class LuxuryCab : public Cab {

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Cab>(*this);
    }

public:
    LuxuryCab(int idNum, Manufacturer manufacturer1, Color color1,
              double tariffVal);

    LuxuryCab();

    ~LuxuryCab();

    int getSizeOfStep();

};


#endif //EX2_LUXURYCAB_H
