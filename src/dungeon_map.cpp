#include "dungeon_map.hpp"

namespace libpmg {
    
DungeonMap::DungeonMap() {
    dungeon_configs = std::make_shared<DungeonMapConfigs>();
    configs_ = dungeon_configs;
}

DungeonMap::DungeonMap(std::shared_ptr<DungeonMap> other) {
    map_uuid_ = other->map_uuid_;
    dungeon_configs = other->dungeon_configs;
    configs_ = dungeon_configs;
    map_ = other->map_;
}

}

