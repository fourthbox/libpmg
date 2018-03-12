#include "location.hpp"

#include "constants.hpp"

namespace libpmg {
    
Location::Location(size_t x, size_t y)
: is_explored_ {false},
cost_ {kDefaultEmptyTileCost},
coords_ {std::make_pair(x, y)}
{}

Location::Location(std::pair<size_t, size_t> xy)
: is_explored_ {false},
cost_ {kDefaultEmptyTileCost},
coords_ {xy}
{}
    
}
