#include "PathCalculator.h"

/**
 * Constructor.
 * @param sourcePoint source point of the way.
 * @return Navigation object.
 */
PathCalculator::PathCalculator(Block *sourcePoint) : source(sourcePoint) {
    path = new deque<Block *>();
    stoppingPoints = new deque<Block *>();
}

/**
 * Default constructor.
 * @return Navigation object.
 */
PathCalculator::PathCalculator() : source(NULL) {

}

/**
 * Constructor.
 * @param pathFromIds stack of ids.
 * @return Navigation object.
 */
PathCalculator::PathCalculator(deque<Block *> *pathFromIds) {
    path = pathFromIds;
    stoppingPoints = new deque<Block *>();
}

/**
 * Destructor.
 */
PathCalculator::~PathCalculator() {
    delete path;
    delete stoppingPoints;
}

/**
 * Get the next location in the way.
 * Set the block to "not visited" state.
 * @return pointer to block.
 */
Block *PathCalculator::getNextLocation() {
    Block *next = path->back();
    path->pop_back();
    // Restore the block's state back to not-visited.
    next->setNotVisited();
    return next;
}

/**
 * Check if need to continue in the path (path is not over).
 * @return true if path is not empty, true otherwise.
 */
bool PathCalculator::hasNextLocation() {
    if (!path->empty()) {
        Block *block = path->back();
        if (block == NULL) {
            path->pop_back();
            return false;
        }
        return true;
    }
    return false;
}

/**
 * Calculate and generate the path between two points.
 * @param block current block in the path and in the recursion.
 * @param startBlock one of the start-block-location in the path.
 * @param currentPath stack that describe one path.
 */
void PathCalculator::calculatePath(Block *block, Block *startBlock,
                               deque<Block *> *currentPath) {
    // If block differs from source push it into the path,
    // and calls recursively to the function with the father.
    if (block != startBlock) {
        currentPath->push_back(block);
        // Set the block back to not visited.
        block->setNotVisited();
        calculatePath(block->Block::getFather(), startBlock, currentPath);
    }
}

/**
 * Add the additional path to the current path.
 * @param additionalPath pointer to stack of pointers to blocks.
 */
void PathCalculator::addToPath(deque<Block *> *additionalPath) {
    // Transfer the current path to dummy stack.
    deque<Block *> *dummy = new deque<Block *>();
    while (!path->empty()) {
        dummy->push_back(path->back());
        path->pop_back();
    }
    delete path;
    path = additionalPath;
    if (!dummy->empty()) {
        Block *stop = NULL;
        path->push_back(stop);
    }
    // Add the current path to the additional path.
    while (!dummy->empty()) {
        path->push_back(dummy->back());
        dummy->pop_back();
    }
    delete dummy;
}

/**
 * Check if there is another stopping point.
 * @return true if the queue of points not empty, false otherwise.
 */
bool PathCalculator::hasNextStoppingPoint() {
    return !stoppingPoints->empty();
}

/**
 * Add the given block to the stopping point.
 * @param block pointer to block, which is a stopping point.
 */
void PathCalculator::addStoppingPoint(Block *block) {
    stoppingPoints->push_back(block);
}

/**
 * Return pointer to the source block.
 * @return get the source location of the path.
 */
Block *PathCalculator::getSource() {
    return source;
}

/**
 * Return pointer to the destination block.
 * @return get the destination location of the path.
 */
Block *PathCalculator::getNextStoppingPoint() {
    Block *block = stoppingPoints->front();
    stoppingPoints->pop_front();
    return block;
}

deque<int> PathCalculator::getOppositePath() {
    deque<int> oppositePath = deque<int>();
    while (hasNextLocation()) {
        oppositePath.push_back(path->back()->getId());
        path->pop_back();
    }
    return oppositePath;
}

/**
 * Create a deque of strings which contain the path as string.
 * @return deque of strings.
 */
deque<string> *PathCalculator::getPathAsString() {
    deque<string> *string_path = new deque<string>();
    while (!path->empty()){
        Block *block = path->front();
        if (block == NULL){
            string_path->push_back("-1,-1");
        } else {
            string_path->push_back(block->getRepresentation());
        }
    }
    return string_path;
}
