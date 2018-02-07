#ifndef LIBPMG_DUNGEON_BUILDER_HPP_
#define LIBPMG_DUNGEON_BUILDER_HPP_

#include "dungeon_map.hpp"
#include "map_builder.hpp"
#include "room.hpp"

namespace libpmg {
    
enum struct PathAlgorithm {
    BREADTH_FIRST_SEARCH,
    DIJKSTRA,
    ASTAR,
    ASTAR_BFS_MIX
};

class DungeonBuilder : public MapBuilder {
public:
    DungeonBuilder();
    
    void InitMap() override;
    void ResetMap() override;
    void GenerateRooms();
    void GenerateCorridors();
    void GenerateDoors();
    void PlaceRoom(Room room);
    void SetMaxRoomSize(std::size_t width, std::size_t height);
    void SetMinRoomSize(std::size_t width, std::size_t height);
    void SetMaxRoomPlacementAttempts(std::size_t attempts);
    void SetDefaultPathAlgorithm(PathAlgorithm algorithm);
    void SetDiagonalCorridors(bool allow);
    void SetMapSize(std::size_t width, std::size_t height) override;
    void SetMaxRooms(std::size_t rooms);
    
    std::shared_ptr<Map> Build() override;
    
private:
    std::shared_ptr<DungeonMap> map_;

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

#endif /* LIBPMG_DUNGEON_BUILDER_HPP_ */
