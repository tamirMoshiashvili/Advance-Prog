#ifndef EX2_BFS_H
#define EX2_BFS_H

#include <boost/serialization/base_object.hpp>
#include "PathCalculator.h"
#include <boost/serialization/deque.hpp>

/**
 * Path calculator with BFS algorithm.
 */
class BFS : public PathCalculator {
private:
    deque<Block *> *bfsQueue;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<PathCalculator>(*this);
        ar & bfsQueue;
    }

public:
    BFS(Block *sourcePoint);

    BFS();

    BFS(deque<Block *> *pathFromIds);

    ~BFS();

    void applyAlgorithm();

private:
    void applyToBlock(Block *block, Block *father);
};


#endif //EX2_BFS_H
