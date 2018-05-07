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
    friend class DungeonBuilder;
    
public:
    DungeonMap();
    ~DungeonMap() {}
    
    /**
     Initializes the DungeonMap copying another map
     @param other The map from which copy the parameters from
     */
    DungeonMap(DungeonMap const &other);
    
    /**
     Initializes the DungeonMap and setup new configs
     @param configs The config file to copy
     */
    DungeonMap(std::shared_ptr<MapConfigs> configs);
    
    /**
     Gets the configuration MapConfigs for this map.
     @return A pointer to the configuration MapConfigs for this map.
     */
    std::shared_ptr<MapConfigs> GetConfigs() override { return std::static_pointer_cast<MapConfigs> (configs_); }
    
    /**
     Gets the map.
     @return A vector containing all the Tile in this map
     */
    std::vector<Tile*> &GetMap() override { return map_; }

    /**
     Gets a reference to the room list of this map.
     @return A reference to room_list_
     */
    constexpr std::vector<Room*> &GetRoomList() { return room_list_; }
    
protected:
    std::vector<Tile*> map_;        /**< All the tiles for this current map */
    std::shared_ptr<DungeonMapConfigs> configs_;    /**< Pointer to the DungeonMapConfigs used to generate this map */
    std::vector<Room*> room_list_;                   /**< A list holding all informations of original generated rooms */
    
};
    
}

#endif /* LIBPMG_DUNGEON_MAP_HPP_ */
