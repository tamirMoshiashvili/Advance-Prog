#ifndef EX2_CITYMAP_H
#define EX2_CITYMAP_H

#include <list>
#include "../Basic/Block/Block.h"
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

/**
 * Represents a map, which is actually a matrix with other data.
 */
class CityMap {
private:
    int width;
    int height;
    vector<vector<Block *> *> grid;
    list<Block *> obstacles;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & width;
        ar & height;
        ar & grid;
        ar & obstacles;
    }

public:
    CityMap(int widthVal, int heightVal);

    CityMap();

    ~CityMap();

    void addObstacle(int x, int y);

    Block *getBlock(int x, int y);

private:
    void addNeighborsToEachBlock();

    void checkForNeighbor(Block *block, int x, int y);
};


#endif //EX2_CITYMAP_H
