#include "location.hpp"

#include "constants.hpp"

namespace libpmg {
    
Location::Location(size_t x, size_t y)
: is_path_explored_ {false},
path_cost_ {kDefaultEmptyTileCost},
coords_ {std::make_pair(x, y)}
{}

Location::Location(std::pair<size_t, size_t> xy)
: is_path_explored_ {false},
path_cost_ {kDefaultEmptyTileCost},
coords_ {xy}
{}
    
}
