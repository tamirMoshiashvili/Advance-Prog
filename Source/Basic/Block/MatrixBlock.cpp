#include "MatrixBlock.h"

/**
 * Constructor.
 * @param p point.
 * @return MatrixBlock object.
 */
MatrixBlock::MatrixBlock(Point p, int id) : Block(id), point(p) {

}

/**
 * Default constructor.
 * @return MatrixBlock object.
 */
MatrixBlock::MatrixBlock() : Block(), point(Point(0, 0)) {

}

/**
 * Destructor.
 */
MatrixBlock::~MatrixBlock() {

}

/**
 * Get the point, which is the location of the block in the matrix.
 * @return point.
 */
Point MatrixBlock::getPoint() {
    return point;
}