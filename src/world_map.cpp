#include "world_map.hpp"

namespace libpmg {
    
WorldMap::WorldMap(std::shared_ptr<WorldMap> other) {
    map_uuid_ = other->map_uuid_;
    configs_ = std::move(other->configs_);
    map_ = std::move(other->map_);
}
        
}
