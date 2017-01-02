#ifndef EX2_LOCATIONDETECTOR_H
#define EX2_LOCATIONDETECTOR_H


#include "CityMap.h"

/**
 * Represents a detector of blocks in the map.
 */
class LocationDetector {
private:
    CityMap *map;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & map;
    }

public:
    LocationDetector(CityMap *cityMap);

    LocationDetector();

    ~LocationDetector();

    Point getLocation(Block *block);

    Block *getBlock(int id);
};


#endif //EX2_LOCATIONDETECTOR_H
