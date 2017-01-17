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

/**
 * Get representation of the block as a string.
 * @return string in format of "x,y" where x,y are coordinates.
 */
string MatrixBlock::getRepresentation() {
    return point.getRepresentation();
}
