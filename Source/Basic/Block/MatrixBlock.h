#ifndef EX2_MATRIXBLOCK_H
#define EX2_MATRIXBLOCK_H


#include "Block.h"

/**
 * Represent a block inside of a matrix.
 */
class MatrixBlock : public Block {
private:
    Point point;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Block>(*this);
        ar & point;
    }

public:
    MatrixBlock(Point p, int id);

    MatrixBlock();

    ~MatrixBlock();

    string getRepresentation();
};


#endif //EX2_MATRIXBLOCK_H
