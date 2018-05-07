#include "dungeon_map.hpp"

namespace libpmg {
    
DungeonMap::DungeonMap() {
    configs_ = std::make_shared<DungeonMapConfigs>();
}
    
DungeonMap::DungeonMap(DungeonMap const &other) {
    map_uuid_ = other.map_uuid_;
    configs_ = other.configs_;
    map_ = other.map_;
}

DungeonMap::DungeonMap(std::shared_ptr<MapConfigs> configs)  {
    auto dungeon_configs {std::static_pointer_cast<DungeonMapConfigs>(configs)};
    
    assert(dungeon_configs != nullptr);
    
    configs_ = dungeon_configs;
}

}

