#ifndef EX2_STANDARDCAB_H
#define EX2_STANDARDCAB_H

#include "Cab.h"

/**
 * Represents a standara cab.
 */
class StandardCab : public Cab {

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Cab>(*this);
    }

public:
    StandardCab(int idNum, Manufacturer manufacturer1, Color color1,
                double tariffVal);

    StandardCab();

    ~StandardCab();

    int getSizeOfStep();
};


#endif //EX2_STANDARDCAB_H
