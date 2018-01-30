#include "location.hpp"

#include "map_configs.hpp"

namespace libpmg {
    
Location::Location(size_t x, size_t y)
: is_explored {false},
cost {kDefaultEmptyTileCost},
coords_ {std::make_pair(x, y)}
{}

Location::Location(std::pair<size_t, size_t> xy)
: is_explored {false},
cost {kDefaultEmptyTileCost},
coords_ {xy}
{}
    
}
