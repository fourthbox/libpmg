#include "world_map.hpp"

namespace libpmg {
    
WorldMap::WorldMap() {
    configs_ = std::make_shared<WorldMapConfigs>();
}

WorldMap::WorldMap(std::shared_ptr<WorldMap> other) {
    map_uuid_ = other->map_uuid_;
    configs_ = other->configs_;
    map_ = other->map_;
}
        
}
