#ifndef EX2_NAVIGATION_H
#define EX2_NAVIGATION_H

#include <queue>
#include <deque>
#include "../../Map/CityMap.h"
#include <boost/serialization/deque.hpp>

/**
 * Represents a navigation that know the shortest path between two blocks.
 */
class PathCalculator {
private:
    Block *source;
    deque<Block *> *stoppingPoints;
    deque<Block *> *path;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & source;
        ar & stoppingPoints;
        ar & path;
    }

public:
    PathCalculator(Block *sourcePoint);

    PathCalculator();

    PathCalculator(deque<Block*> *pathFromIds);

    virtual ~PathCalculator();

    //TODO: delete.
    Block *getNextLocation();

    //TODO: delete.
    bool hasNextLocation();

    virtual void applyAlgorithm()=0;

    void addStoppingPoint(Block *block);

    deque<int> getOppositePath();

    deque<string> *getPathAsString();

private:
    virtual void applyToBlock(Block *block, Block *father)=0;

protected:
    void calculatePath(Block *block, Block *startBlock,
                       deque<Block *> *currentPath);

    void addToPath(deque<Block *> *additionalPath);

    Block *getSource();

    Block *getNextStoppingPoint();

    bool hasNextStoppingPoint();
};


#endif //EX2_NAVIGATION_H
