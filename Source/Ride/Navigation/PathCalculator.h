#ifndef EX2_PATHCALCULATOR_H
#define EX2_PATHCALCULATOR_H

#include <queue>
#include <deque>
#include "../../Map/CityMap.h"
#include <boost/serialization/deque.hpp>

/**
 * Represents an object that know the shortest path between two blocks.
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

    PathCalculator(deque<Block *> *pathFromIds);

    virtual ~PathCalculator();

    virtual void applyAlgorithm()=0;

    void addStoppingPoint(Block *block);

    deque<string> *getPathAsString();

    bool isValidRide();

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


#endif //EX2_PATHCALCULATOR_H
