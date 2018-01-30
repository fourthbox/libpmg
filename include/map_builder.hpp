/*
 * This is the main class managing the creation of a Map. After initialization, the user should call the needed Set___ methods
 * then the InitMap(), GenerateRooms() and GenerateCorridors() in this order.
 */

#ifndef LIBPMG_MAP_BUILDER_HPP_
#define LIBPMG_MAP_BUILDER_HPP_

#include <memory>

#include "map.hpp"
#include "room.hpp"

namespace libpmg {

enum struct PathAlgorithm {
    BREADTH_FIRST_SEARCH,
    DIJKSTRA,
    ASTAR,
    ASTAR_BFS_MIX
};

class MapBuilder {
public:
    MapBuilder();
    
    MapBuilder& InitMap();
    MapBuilder& ResetMap();
    MapBuilder& GenerateRooms();
    MapBuilder& GenerateCorridors();
    MapBuilder& GenerateDoors();
    MapBuilder& PlaceRoom(Room room);
    MapBuilder& SetMaxRoomSize(std::size_t width, std::size_t height);
    MapBuilder& SetMinRoomSize(std::size_t width, std::size_t height);
    MapBuilder& SetMaxRoomPlacementAttempts(std::size_t attempts);
    MapBuilder& SetDefaultPathAlgorithm(PathAlgorithm algorithm);
    MapBuilder& SetDiagonalCorridors(bool allow);
    MapBuilder& SetMapSize(std::size_t width, std::size_t height);
    MapBuilder& SetMaxRooms(std::size_t rooms);
    
    std::shared_ptr<Map> Build();
    
private:
    std::shared_ptr<Map> map_;
    PathAlgorithm default_path_algorithm_;
    bool allow_diagonal_corridors_;
    
    void ConnectRooms(Room room1, Room room2);
    void PlaceRect(Rect rect,
                   std::initializer_list<std::shared_ptr<Tag>> tag);
    void RemoveRect(Rect rect,
                    std::initializer_list<std::shared_ptr<Tag>> tag);
    bool CanPlaceRect(Rect rect,
                      std::initializer_list<std::shared_ptr<Tag>> black_list,
                      std::initializer_list<std::shared_ptr<Tag>> white_list = {});
    void UpdateRect(Rect rect,
                    std::initializer_list<std::shared_ptr<Tag>> to_insert,
                    std::initializer_list<std::shared_ptr<Tag>> to_remove);
    void PlaceDoor(std::shared_ptr<Tile> tile);
    
    bool IsDiagonalCorridor();
    
};

}
#endif /* LIBPMG_MAP_BUILDER_HPP_ */
