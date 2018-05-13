/**
 @file dungeon_builder.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_DUNGEON_BUILDER_HPP_
#define LIBPMG_DUNGEON_BUILDER_HPP_

#include "dungeon_map.hpp"
#include "map_builder.hpp"
#include "room.hpp"

namespace libpmg {
   
/**
 Define 4 different types of path finding algorithm.
 */
enum struct PathAlgorithm {
    BREADTH_FIRST_SEARCH,
    DIJKSTRA,
    ASTAR,
    ASTAR_BFS_MIX
};

/**
 MapBuilder implementation for generating dungeon maps.
 */
class DungeonBuilder : public MapBuilder {
public:
    DungeonBuilder();
    
    /**
     Initializes every tile in the map. It must me called after generating room, corridors and doors.
     */
    void InitMap() override;
    
    /**
     Reset the map and re-initializes it.
     @param keep_configs If true the new map will keep the same configs.
     */
    void ResetMap(bool keep_configs) override;
    
    /**
     Generate a random number of rooms and digs them in the map.
     */
    void GenerateRooms();
    
    /**
     Generate a corridor system that connects every room, and digs it in the map.
     */
    void GenerateCorridors();
    
    /**
     Generate doors in every elegible tile.
     Tiles are elegible for doors only if they are adjacent of 2 wall tiles and 2 empty tiles, both opposed to each other.
     */
    void GenerateDoors();
    
    /**
     Generate a variable number of up and down doors, embedded into walls.
     @param up Number of stairs up.
     @param down Number of staris down.
     */
    void GenerateWallStairs(size_t up, size_t down);
    
    /**
     Generate a variable number of up and down stairs, dig into the ground.
     @param up Number of stairs up.
     @param down Number of staris down.
     @param only_in_rooms If set to true locations will only be picked inside of rooms
     @param dig_space If set to true will remove walls from the 8 tiles adjacent to every stair
     */
    void GenerateGroundStairs(size_t up, size_t down, bool only_in_rooms, bool dig_space);
    
    /**
     Place a single room in a map without checking for collisions.
     @param room Pointer to the room to place
     */
    void PlaceRoom(Room *room);
    
    /**
     Set the max room size into the configs.
     @param width Max room width
     @param height Max room height
     */
    void SetMaxRoomSize(std::size_t width, std::size_t height);
    
    /**
     Set the min room size into the configs.
     @param width Min room width
     @param height Min room height
     */
    void SetMinRoomSize(std::size_t width, std::size_t height);
    
    /**
     Set the max attempts the algorithm should try to dig a room without collisions.
     @param attempts Max number of attemprs
     */
    void SetMaxRoomPlacementAttempts(std::size_t attempts);
    
    /**
     Set algorithm used to generate corridors.
     Corridors try not to collide with rooms and other corridors. You can specify which algorithm will be used to connect rooms with eachother.
     @param algorithm The algorithm used
     */
    void SetDefaultPathAlgorithm(PathAlgorithm const &algorithm);
    
    /**
     Set wether diagonal corridors are allowed
     @param allow Wether diagonal corridors should be allowed
     */
    void SetDiagonalCorridors(bool allow);
    
    /**
     Set the size of the map.
     @param width Map width
     @param height Map height
     */
    void SetMapSize(std::size_t width, std::size_t height) override;
    
    /**
     Set max number of rooms to be generated.
     @param rooms Max number of room
     */
    void SetMaxRooms(std::size_t rooms);
    
    /**
     Build the map and returns a pointer.
     @return A pointer to the built map.
     */
    std::shared_ptr<Map> Build() override;
        
private:
    std::shared_ptr<DungeonMap> map_;       /**< The map. */
    PathAlgorithm default_path_algorithm_;  /**< The default path finder algorithm used for generating corridors. */
    bool allow_diagonal_corridors_;         /**< Should the builder generate diagonal corridors? */
    
    /**
     Connect 2 rooms with a corridor using path finding defined rules to avoid collisions.
     @param room1 The first room
     @param room2 The second room
     */
    void ConnectRooms(Room const &room1, Room const &room2);
    
    /**
     Add the specified tag to all the tiles in the specified Rect.
     @param rect The rect
     @param tag The tag to add
     */
    void PlaceRect(Rect const &rect,
                   std::initializer_list<std::shared_ptr<Tag>> tag);
    
    /**
     Removes the specified tag to all the tiles in the specified Rect.
     @param rect The rect
     @param tag The tag to remove
     */
    void RemoveRect(Rect const &rect,
                    std::initializer_list<std::shared_ptr<Tag>> tag);
    
    /**
     Checks whether the Rect can be placed.
     The Rect can be placed only if any tile within the specified rect has at zero or more black listed tiles and no white listed tiles.
     @param rect The rect
     @param black_list The list of tiles that cannot be within the rect
     @param white_list The list of tiles that can stay in the rect, even if they have multiple blacklisted tags
     @return True if the Rect can be placed, false otherwise
     */
    bool CanPlaceRect(Rect const &rect,
                      std::initializer_list<std::shared_ptr<Tag>> black_list,
                      std::initializer_list<std::shared_ptr<Tag>> white_list = {});
    /**
     Changes the tags in the tiles within the specified Rect
     @param rect The rect
     @param to_insert Tags that will be added to the tiles
     @param to_remove Tags that will be removed form the tiles
     */
    void UpdateRect(Rect const &rect,
                    std::initializer_list<std::shared_ptr<Tag>> to_insert,
                    std::initializer_list<std::shared_ptr<Tag>> to_remove);
    
    /**
     Place a door if the tile is eligible.
     In order be eligible for a door a tile must be adjacent to no more then two wall tiles, and they must be opposite to eachother.x
     @param tile A pointer to the tile that will host the door
     */
    void PlaceDoor(Tile *tile);
    
    /**
     Place a stair tile, if eligible.
     In order to be eligible for stairs, a tile must have at least 3 adjacent tiles (in four cardinal directions)
     @param tile A pointer to the tile that will host the stair
     @param is_upstairs If set to true, the stair will be upstairs. It will be downstairs otherwise.
     */
    void PlaceStairs(Tile *tile, bool is_upstairs);
    
    /**
     Check if the corridor to be placed should be diagonal.
     @return False if allow_diagonal_corridors_ is false, otherwise a random boolean
     */
    bool IsDiagonalCorridor();
    
};
    
}

#endif /* LIBPMG_DUNGEON_BUILDER_HPP_ */
