#include "BFS.h"

/**
 * Constructor.
 * @param sourcePoint source point of the way.
 * @return BFS object.
 */
BFS::BFS(Block *sourcePoint) : PathCalculator(sourcePoint) {
    bfsQueue = new deque<Block *>();
}

/**
 * Default constructor.
 * @return BFS object.
 */
BFS::BFS() : PathCalculator(), bfsQueue(NULL) {

}

/**
 * Constructor.
 * @param pathFromIds stack that represents the path of the navigation.
 * @return BFS object.
 */
BFS::BFS(deque<Block *> *pathFromIds) : PathCalculator(pathFromIds),
                                        bfsQueue(NULL) {

}

/**
 * Destructor.
 */
BFS::~BFS() {
    delete bfsQueue;
}

/**
 * Apply the BFS algorithm to generate the path.
 */
void BFS::applyAlgorithm() {
    Block *source = getSource();
    while (hasNextStoppingPoint()) {
        // Handle the source block according to the BFS algorithm.
        Block *destination = getNextStoppingPoint();
        source->setVisited();
        queue<Block *> *usedBlocks = new queue<Block *>();
        bfsQueue->push_back(source);
        while (!bfsQueue->empty()) {
            // Get and pop the block from the queue.
            Block *current = bfsQueue->front();
            usedBlocks->push(current);
            bfsQueue->pop_front();
            if (current == destination) {
                // We have reached to the destination point,
                // so we can generate the path.
                std::deque<Block *> *currentPath = new deque<Block *>();
                calculatePath(current, source, currentPath);
                addToPath(currentPath);
                source->setNotVisited();
                break;
            } else {
                // Push the neighbors to the queue and continue the algorithm.
                std::list<Block *> neighbors = current->Block::getNeighbors();
                for (list<Block *>::iterator it = neighbors.begin();
                     it != neighbors.end(); ++it) {
                    applyToBlock(*it, current);
                }
            }
        }
        // Push what left from the queue to the used blocks list.
        while (!bfsQueue->empty()) {
            Block *b = bfsQueue->front();
            bfsQueue->pop_front();
            usedBlocks->push(b);
        }
        // Reset all the blocks we used to generate the path.
        while (!usedBlocks->empty()) {
            Block *usedBlock = usedBlocks->front();
            usedBlocks->pop();
            usedBlock->setNotVisited();
        }
        delete usedBlocks;
        // Initialize the bfs-queue.
        delete bfsQueue;
        bfsQueue = new deque<Block *>();
        // Update the start-block.
        source = destination;
    }
}

/**
 * Set the father of the given block to the given father,
 * and notify the block that it has been visited.
 * @param block pointer to block.
 * @param father its father.
 */
void BFS::applyToBlock(Block *block, Block *father) {
    // Validate the block and check if we can apply the algorithm on it.
    if (!block->Block::checkIfVisited()) {
        bfsQueue->push_back(block);
        block->Block::setFather(father);
        block->Block::setVisited();
    }
}
