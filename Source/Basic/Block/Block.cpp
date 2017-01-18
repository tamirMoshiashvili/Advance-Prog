#include "Block.h"

/**
 * Constructor.
 * @return Block object.
 */
Block::Block(int id) : Recognizable(id), visited(false), father(NULL) {

}

/**
 * Default constructor.
 * @return Block object.
 */
Block::Block() : Recognizable(), visited(false), father(NULL) {

}

/**
 * Destructor.
 */
Block::~Block() {

}

/**
 * Set the father of the current block.
 * @param fatherBlock pointer to block.
 */
void Block::setFather(Block *fatherBlock) {
    father = fatherBlock;
}

/**
 * Get the father of the current block.
 * @return pointer to block.
 */
Block *Block::getFather() {
    return father;
}

/**
 * Check if the current block has been visited before.
 * @return true if it is, false otherwise.
 */
bool Block::checkIfVisited() {
    return visited;
}

/**
 * Notify the block that it has been visited.
 */
void Block::setVisited() {
    visited = true;
}

/**
 * Set the state of this block to "not-visited",
 * means that no one visited this block yet.
 */
void Block::setNotVisited() {
    visited = false;
}

/**
 * Add a neighbor to the current block.
 * @param neighbor pointer to block.
 */
void Block::addNeighbor(Block *neighbor) {
    neighbors.push_back(neighbor);
}

/**
 * Get the list of block-neighbors of the current block.
 * @return list of pointers to blocks.
 */
list<Block *> Block::getNeighbors() {
    return neighbors;
}

/**
 * Check if the given block equal to the current.
 * @param other pointer to block.
 * @return
 */
bool Block::operator==(Block* other) {
    return this->getId() == other->getId();
}
