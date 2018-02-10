/**
 @file grid.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_GRID_HPP_
#define LIBPMG_GRID_HPP_

#include <vector>

#include "location.hpp"

namespace libpmg {

/**
 Represent a 4 and 8 direction movement.
 */
enum struct MoveDirections {
    FOUR_DIRECTIONAL = 4,
    EIGHT_DIRECTIONAL = 8
};

/**
 Pure virtual class representing a grid of tiles
 */
class Grid {
public:
    
    /**
     Gets the tiles adjacent to the selected tile.
     @param location The tile to get the neighbors from
     @param dir Whether getting only tiles adjacent on cardinal directions, or diagonal tiles
     @return A vector of pointers to the neighbor tiles
     */
    virtual std::vector<std::shared_ptr<Location>> GetNeighbors(std::shared_ptr<Location> location, MoveDirections dir) = 0;
    
    /**
     Resets all the costs calculated by the path finding algorithm.
     */
    virtual void ResetLocationCosts() = 0;
    
    /**
     Resets all the flags added by the path finding algorithm.
     */
    virtual void ResetPathFlags() = 0;

};

}
#endif /* LIBPMG_GRID_HPP_ */
