#ifndef LIBPMG_DUNGEON_MAP_HPP_
#define LIBPMG_DUNGEON_MAP_HPP_

#include "map.hpp"
#include "room.hpp"

namespace libpmg {
    
class DungeonMapConfigs : public MapConfigs {
public:
    DungeonMapConfigs()
    : rooms_ {20},
    max_room_placement_attempts_ {10},
    max_room_width_ {12},
    max_room_height_ {12},
    min_room_width_ {4},
    min_room_height_ {4}
    {}
    
    std::size_t rooms_;
    std::size_t max_room_placement_attempts_;
    std::size_t max_room_width_;
    std::size_t max_room_height_;
    std::size_t min_room_width_;
    std::size_t min_room_height_;
};

class DungeonMap : public Map {
    friend class DungeonBuilder;
    
public:
    DungeonMap();
    DungeonMap(std::shared_ptr<DungeonMap> other);
        
    std::shared_ptr<DungeonMapConfigs> dungeon_configs;
    
private:
    std::vector<Room> room_list_;
    
};
    
}

#endif /* LIBPMG_DUNGEON_MAP_HPP_ */
