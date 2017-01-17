#ifndef EX2_BLOCK_H
#define EX2_BLOCK_H


#include "../Point.h"
#include "../Recognizable.h"
#include <list>
#include <boost/serialization/list.hpp>
#include <boost/serialization/base_object.hpp>

using namespace std;

/**
 * Represents a block that holds some data.
 */
class Block : public Recognizable {
private:
    bool visited;
    Block *father;
    list<Block *> neighbors;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Recognizable>(*this);
        ar & visited;
        ar & father;
        ar & neighbors;
    }

public:
    Block(int id);

    Block();

    virtual ~Block();

    void setFather(Block *fatherBlock);

    Block *getFather();

    bool checkIfVisited();

    void setVisited();

    void setNotVisited();

    void addNeighbor(Block *neighbor);

    list<Block *> getNeighbors();

    bool operator==(Block* other);

    virtual string getRepresentation()=0;
};


#endif //EX2_BLOCK_H
