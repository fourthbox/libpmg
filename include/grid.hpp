#ifndef LIBPMG_GRID_HPP_
#define LIBPMG_GRID_HPP_

#include <vector>

#include "location.hpp"

namespace libpmg {

enum struct MoveDirections {
    FOUR_DIRECTIONAL = 4,
    EIGHT_DIRECTIONAL = 8
};

class Grid {
public:    
    virtual std::vector<std::shared_ptr<Location>> GetNeighbors(std::shared_ptr<Location> location, MoveDirections dir) = 0;
    virtual void ResetLocationCosts() = 0;
};

}
#endif /* LIBPMG_GRID_HPP_ */
