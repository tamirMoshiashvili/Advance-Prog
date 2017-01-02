#ifndef EX2_BFS_H
#define EX2_BFS_H

#include <boost/serialization/base_object.hpp>
#include "Navigation.h"
#include <boost/serialization/deque.hpp>

/**
 * Navigation system that work with BFS algorithm.
 */
class BFS : public Navigation {
private:
    deque<Block *> *bfsQueue;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Navigation>(*this);
        ar & bfsQueue;
    }

public:
    BFS(Block *sourcePoint);

    BFS();

    BFS(deque<Block*> *pathFromIds);

    ~BFS();

    void applyAlgorithm();

private:
    void applyToBlock(Block *block, Block *father);

};


#endif //EX2_BFS_H
