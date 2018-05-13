/**
 @file dungeon_map.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_DUNGEON_MAP_HPP_
#define LIBPMG_DUNGEON_MAP_HPP_

#include "map.hpp"
#include "room.hpp"

namespace libpmg {
    
/**
 Struct that holds the DungeonMap configuration values
 */
struct DungeonMapConfigs : public MapConfigs {
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

/**
 Class that holds and manages all tile informations for a dungeon map.
 */
class DungeonMap : public Map {
    
public:
    DungeonMap();
    ~DungeonMap() {}
    
    /**
     Initializes the DungeonMap copying another map
     @param other The map from which copy the parameters from
     */
    DungeonMap(DungeonMap &other);
    
    /**
     Initializes the DungeonMap and setup new configs
     @param configs The config file to copy
     */
    DungeonMap(MapConfigs &configs);
    
    /**
     Gets the configuration MapConfigs for this map.
     @return A reference to the configuration MapConfigs for this map.
     */
    MapConfigs &GetConfigs() override { return *configs_; }
    
    /**
     Gets the map.
     @return A pointer to the vector containing all the Tile in this map
     */
    std::unique_ptr<std::vector<std::unique_ptr<Tile>>> &GetMap() override { return map_; }

    /**
     Gets a reference to the room list of this map.
     @return A reference to room_list_
     */
    constexpr std::vector<std::unique_ptr<Room>> &GetRoomList() { return room_list_; }
    
protected:
    std::unique_ptr<std::vector<std::unique_ptr<Tile>>> map_;        /**< All the tiles for this current map */
    std::unique_ptr<DungeonMapConfigs> configs_;    /**< Pointer to the DungeonMapConfigs used to generate this map */
    std::vector<std::unique_ptr<Room>> room_list_;                   /**< A list holding all informations of original generated rooms */
    
};
    
}

#endif /* LIBPMG_DUNGEON_MAP_HPP_ */
