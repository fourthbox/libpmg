#include "dungeon_map.hpp"

namespace libpmg {
    
DungeonMap::DungeonMap() {
    configs_ = std::make_unique<DungeonMapConfigs>();
    map_ = std::make_unique<std::vector<std::unique_ptr<Tile>>>();
}
    
DungeonMap::DungeonMap(DungeonMap &other) {
    map_uuid_ = other.map_uuid_;
    configs_ = std::move(other.configs_);
    map_ = std::move(other.map_);
}

DungeonMap::DungeonMap(MapConfigs &configs)  {    
    configs_ = std::make_unique<DungeonMapConfigs>((DungeonMapConfigs&) configs);
}

}

