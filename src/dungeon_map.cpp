#include "dungeon_map.hpp"

namespace libpmg {
    
DungeonMap::DungeonMap() {
    configs_ = std::make_shared<DungeonMapConfigs>();
}

DungeonMap::DungeonMap(std::shared_ptr<DungeonMap> other) {
    map_uuid_ = other->map_uuid_;
    configs_ = other->configs_;
    map_ = other->map_;
}

}

