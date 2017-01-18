#include "CityMap.h"
#include "../Basic/Block/MatrixBlock.h"

/**
 * Constructor.
 * @param widthVal width of the map.
 * @param heightVal height of the map.
 * @return CityMap object.
 */
CityMap::CityMap(int widthVal, int heightVal)
        : width(widthVal), height(heightVal) {
    // Creates all the blocks.
    int id = 0;
    for (int i = 0; i < width; ++i) {
        vector<Block *> *row = new vector<Block *>();
        for (int j = 0; j < height; ++j) {
            MatrixBlock *block = new MatrixBlock(Point(i, j), id);
            (*row).push_back(block);
            ++id;
        }
        grid.push_back(row);
    }
    // Add to each block its neighbors.
    addNeighborsToEachBlock();
}

/**
 * Default constructor.
 * @return CityMap object.
 */
CityMap::CityMap() : width(0), height(0) {

}

/**
 * Destructor.
 */
CityMap::~CityMap() {
    // Run through the matrix and delete all the pointers.
    for (int i = 0; i < width; ++i) {
        vector<Block *> *row = grid.back();
        grid.pop_back();
        for (int j = 0; j < height; ++j) {
            Block *block = row->back();
            row->pop_back();
            delete block;
        }
        delete row;
    }
}

/**
 * Add to each node its neighbors.
 */
void CityMap::addNeighborsToEachBlock() {
    // Run through the grid.
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            // Check for all the possible neighbors.
            Block *block = getBlock(i, j);
            checkForNeighbor(block, i - 1, j);
            checkForNeighbor(block, i, j + 1);
            checkForNeighbor(block, i + 1, j);
            checkForNeighbor(block, i, j - 1);
        }
    }
}

/**
 * Check that the given coordinates (x,y) represent a block in the matrix,
 * if so, the new block is a neighbor of the given one.
 * @param block Block object.
 * @param x number on x-axis.
 * @param y number on y-axis.
 */
void CityMap::checkForNeighbor(Block *block, int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        // The given coordinates are inside the matrix, so add the block.
        block->addNeighbor(getBlock(x, y));
    }
}

/**
 * Add an obstacle to the map.
 * An obstacle will be a block which will be marked as "visited",
 * by that, when we reach an obstacle, the program will not operate on it.
 * @param x x-coordinate on the grid.
 * @param y y-coordinate on the grid.
 */
void CityMap::addObstacle(int x, int y) {
    Block *obstacle = getBlock(x, y);
    obstacle->setVisited();
    obstacles.push_back(obstacle);
}

/**
 * Get the block which is in the given coordinates.
 * @param x value on the x axis.
 * @param y value on the y axis.
 * @return pointer to block.
 */
Block *CityMap::getBlock(int x, int y) {
    return (*grid[x])[y];
}