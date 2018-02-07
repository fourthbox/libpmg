#include "world_map.hpp"

namespace libpmg {
    
WorldMap::WorldMap() {
    world_configs = std::make_shared<WorldMapConfigs>();
    configs_ = world_configs;
}

WorldMap::WorldMap(std::shared_ptr<WorldMap> other) {
    map_uuid_ = other->map_uuid_;
    world_configs = other->world_configs;
    configs_ = world_configs;
    map_ = other->map_;
}
    
}
