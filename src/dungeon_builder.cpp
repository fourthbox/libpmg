#include "dungeon_builder.hpp"

#include <cassert>
#include <queue>

#define FLOOR_TAG_ TagManager::GetInstance().floor_tag_
#define WALL_TAG_ TagManager::GetInstance().wall_tag_
#define DOOR_TAG_ TagManager::GetInstance().door_tag_
#define UPSTAIRS_TAG_ TagManager::GetInstance().upstairs_tag_
#define DOWNSTAIRS_TAG_ TagManager::GetInstance().downstairs_tag_

#include "constants.hpp"
#include "dungeon_map.hpp"
#include "rnd_manager.hpp"
#include "utils.hpp"

namespace libpmg {
    
typedef std::shared_ptr<Tag> Tag_p;
typedef std::unique_ptr<std::unordered_map<Location*, Location*>> LocationMap_up;
    
DungeonBuilder::DungeonBuilder()
: default_path_algorithm_ {PathAlgorithm::ASTAR_BFS_MIX},
allow_diagonal_corridors_ {true} {
    map_ = std::make_unique<DungeonMap>();
    
    assert(map_->GetMap()->empty());
}

std::unique_ptr<Map> &DungeonBuilder::Build() {
    if (map_->GetMap()->empty()) {
        Utils::LogError("DungeonBuilder::Build", "Map has not been not initialized.\nAborting...");
        abort();
    }
        
    return map_;
}
    
void DungeonBuilder::SetDefaultPathAlgorithm(PathAlgorithm const &algorithm) {
    assert (map_->GetMap()->empty());

    default_path_algorithm_ = algorithm;
}

void DungeonBuilder::SetDiagonalCorridors(bool allow) {
    assert (map_->GetMap()->empty());

    allow_diagonal_corridors_ = allow;
}

void DungeonBuilder::SetMapSize(size_t width, size_t height) {
    assert (map_->GetMap()->empty());

    map_->GetConfigs().map_width_ = width;
    map_->GetConfigs().map_height_ = height;
}

void DungeonBuilder::SetMaxRooms(size_t rooms) {
    assert (map_->GetMap()->empty());

    ((DungeonMapConfigs&)map_->GetConfigs()).rooms_ = rooms;
}
    
bool DungeonBuilder::IsDiagonalCorridor() {
    if (allow_diagonal_corridors_)
        return RndManager::GetInstance().GetRandomUintFromRange(0,1);
    
    return false;
}

/**
 Connects two rooms with a corridor, generated with a random combination of Astar and BreadthFirstSearch.
 @param room1 The first room.
 @param room2 The second room.
 */
void DungeonBuilder::ConnectRooms(Room const &room1, Room const &room2) {
    Location *start {map_->GetTile(room1.GetRndCoords())};
    Location *end {map_->GetTile(room2.GetRndCoords())};
    
    LocationMap_up path {nullptr};
    switch (default_path_algorithm_) {
        case PathAlgorithm::BREADTH_FIRST_SEARCH:
            path = Utils::BreadthFirstSearch(
                                             start->GetXY(),
                                             end->GetXY(),
                                             map_.get(),
                                             IsDiagonalCorridor(),
                                             MoveDirections::FOUR_DIRECTIONAL);
            break;
        case PathAlgorithm::DIJKSTRA:
            path = Utils::Dijkstra(
                                   start->GetXY(),
                                   end->GetXY(),
                                   map_.get(),
                                   MoveDirections::FOUR_DIRECTIONAL);
            break;
        case PathAlgorithm::ASTAR:
            path = Utils::Astar(
                                start->GetXY(),
                                end->GetXY(),
                                map_.get(),
                                MoveDirections::FOUR_DIRECTIONAL);
            break;
        case PathAlgorithm::ASTAR_BFS_MIX:
        default:
            if (RndManager::GetInstance().GetRandomUintFromRange(0,1))
                path = Utils::Astar(
                                    start->GetXY(),
                                    end->GetXY(),
                                    map_.get(),
                                    MoveDirections::FOUR_DIRECTIONAL);
            else
                path = Utils::BreadthFirstSearch(
                                                 start->GetXY(),
                                                 end->GetXY(),
                                                 map_.get(),
                                                 IsDiagonalCorridor(),
                                                 MoveDirections::FOUR_DIRECTIONAL);
            break;
    }
    
    assert(path != nullptr);
    
    // Returns the Location from which coords it come from
    auto calculate_from_where = [=] (Location *coords, LocationMap_up &came_from) -> Location* {
        for (auto const &kv : *came_from) {
            if (kv.first == coords)
                return kv.second;
        }
        
        Utils::LogError("Astar", "Broken path");
        abort();
    };
    
    // Flags the generated corridor with the proper tags
    while (end != start) {
        map_->GetTile(end->GetXY())->UpdateTags({FLOOR_TAG_}, {WALL_TAG_});
        end = calculate_from_where(end, path);
    }
    
    // Applies a cost to every tile in a room or a corridor, and to their neighbors, in order to
    // avoid corridors intersecating too much
    for (auto const &tile : *map_->GetMap()){
        if (tile->Taggable::HasTag(FLOOR_TAG_)) {
            tile->path_cost_ = kDefaultWallTileCost;
            for (auto const &nei : map_->GetNeighbors(tile.get(), MoveDirections::EIGHT_DIRECTIONAL))
                nei->path_cost_ = kDefaultWallTileCost;
        }
    }
}

void DungeonBuilder::GenerateCorridors() {
    auto dungeon_map {(DungeonMap*)map_.get()};
    
    if (dungeon_map->GetRoomList().empty() || map_->GetMap()->empty()) {
        Utils::LogWarning("DungeonBuilder::placeCorridors", "There are no rooms, or no free space. Skipping corridor generation...");
        return;
    }
    
    for (auto i {0}; i < dungeon_map->GetRoomList().size() - 1; i++)
        ConnectRooms(*dungeon_map->GetRoomList().at(i), *dungeon_map->GetRoomList().at(i + 1));
    
    // In case of odd rooms
    if (dungeon_map->GetRoomList().size()%2 != 0)
        ConnectRooms(
                     *dungeon_map->GetRoomList().at(dungeon_map->GetRoomList().size()-2),
                     *dungeon_map->GetRoomList().at(dungeon_map->GetRoomList().size()-1));
}
    
void DungeonBuilder::InitMap() {
    for (auto i {0}; i < map_->GetConfigs().map_height_; i++) {
        for (auto j {0}; j < map_->GetConfigs().map_width_; j++){
            std::initializer_list<std::shared_ptr<Tag>> tags = {WALL_TAG_};
            map_->GetMap()->push_back(std::make_unique<Tile>(j, i, tags));
        }
    }
}

void DungeonBuilder::ResetMap(bool keep_configs) {
    auto configs {(DungeonMapConfigs&)map_->GetConfigs()};
    map_ = std::make_unique<DungeonMap>(configs);
    this->InitMap();
}

void DungeonBuilder::GenerateRooms() {
    if (map_->GetMap()->empty()) {
        Utils::LogWarning("DungeonBuilder::GenerateRooms", "m_map has not been not initialized. Initializing now...");
        InitMap();
    }
    
    auto dungeon_configs {(DungeonMapConfigs&)map_->GetConfigs()};
    
    //  Generate each room.
    for (auto i {0}; i < dungeon_configs.rooms_; i++) {
        for (auto j {0}; j < dungeon_configs.max_room_placement_attempts_; j++) {
            
            //          Get random rect
            auto rndRect {Rect::GetRndRect(1, dungeon_configs.map_width_-1,
                                           1, dungeon_configs.map_height_-1,
                                           dungeon_configs.min_room_width_, dungeon_configs.max_room_width_,
                                           dungeon_configs.min_room_height_, dungeon_configs.max_room_height_)};
            
            if (CanPlaceRect(++rndRect, {FLOOR_TAG_})) {
                auto new_room {std::make_unique<Room> (--rndRect)};
                PlaceRoom(new_room);
                break;
            }
            
            if (j == dungeon_configs.max_room_placement_attempts_ - 1)
                Utils::LogDebug( "DungeonBuilder", "Last room placement attempt failed. Moving on...");
        }
    }
}

void DungeonBuilder::PlaceDoor(Tile *tile) {
    assert (tile != nullptr);
    
    if (!tile->HasTag(FLOOR_TAG_))
        return;
    
    //    Get the four neighbours
    auto neis {map_->GetNeighbors(tile, MoveDirections::FOUR_DIRECTIONAL)};
    
    if (neis.size() < 4)
        return;
    
    auto wall_count {0};
    for (auto const &nei : neis) {
        // If one neighbour has a door, exit
        if (nei->HasTag(DOOR_TAG_))
            return;
        
        // Count how many neighbours has walls
        if (nei->HasTag(WALL_TAG_))
            wall_count++;
    }
    
    // If more then 2 neighbours has doors, there can't be two opposing wall tiles, therefore no place for a door
    if (wall_count >= 3)
        return;
    
    // Checks if the 2 neighbouring wall tiles are opposed to eachother
    if ((map_->GetTile(tile->GetX(), tile->GetY()-1)->HasTag(WALL_TAG_)
         && map_->GetTile(tile->GetX(), tile->GetY()+1)->HasTag(WALL_TAG_))
        || (map_->GetTile(tile->GetX()-1, tile->GetY())->HasTag(WALL_TAG_)
            && map_->GetTile(tile->GetX()+1, tile->GetY())->HasTag(WALL_TAG_)))
        tile->AddTag(DOOR_TAG_);
}
    
void DungeonBuilder::PlaceStairs(Tile *tile, bool is_upstairs) {
    assert (tile != nullptr);
    
    if (is_upstairs)
        tile->UpdateTags({UPSTAIRS_TAG_}, {WALL_TAG_});
    else
        tile->UpdateTags({DOWNSTAIRS_TAG_}, {WALL_TAG_});
}

void DungeonBuilder::GenerateDoors() {
    auto dungeon_map {(DungeonMap*)map_.get()};
    
    if (dungeon_map->GetRoomList().empty() || map_->GetMap()->empty()) {
        Utils::LogWarning("DungeonBuilder::GenerateDoors", "There are no rooms, or no free space. Skipping door generation...");
        return;
    }
    
    // Scan the borders fo the rooms form tiles eligible for doors
    for (auto const &room : dungeon_map->GetRoomList()) {
        Rect rect {room->Area::GetRect()};
        rect++;

        for (auto w {0}; w < rect.GetWidth(); w++) {
            auto tile {map_->GetTile(rect.GetX() + w, rect.GetY())};
            PlaceDoor(tile);
            
            tile = map_->GetTile(rect.GetX() + w, rect.GetY() + rect.GetHeight()-1);
            PlaceDoor(tile);
        }
        
        for (auto h {0}; h < rect.GetHeight(); h++) {
            auto tile {map_->GetTile(rect.GetX(), rect.GetY() + h)};
            PlaceDoor(tile);
            
            tile = map_->GetTile(rect.GetX() + rect.GetWidth()-1, rect.GetY() + h);
            PlaceDoor(tile);
        }
    }
}
    
void DungeonBuilder::GenerateWallStairs(size_t up, size_t down) {
    auto dungeon_map {(DungeonMap*)map_.get()};

    if (dungeon_map->GetRoomList().empty() || map_->GetMap()->empty()) {
        Utils::LogWarning("DungeonBuilder::GenerateWallStairs", "There are no rooms, or no free space. Skipping door generation...");
        return;
    }
    
    std::vector<Tile*> eligeble_tiles;

    // Scan the borders fo the rooms form tiles eligible for stairs
    for (auto const &room : dungeon_map->GetRoomList()) {
        Rect rect {room->Area::GetRect()};
        rect++;

        for (auto w {1}; w < rect.GetWidth()-1; w++) {
            eligeble_tiles.push_back(map_->GetTile(rect.GetX() + w, rect.GetY()));

            eligeble_tiles.push_back(map_->GetTile(rect.GetX() + w, rect.GetY() + rect.GetHeight()-1));
        }
        
        for (auto h {1}; h < rect.GetHeight()-1; h++) {
            eligeble_tiles.push_back(map_->GetTile(rect.GetX(), rect.GetY() + h));

            eligeble_tiles.push_back(map_->GetTile(rect.GetX() + rect.GetWidth()-1, rect.GetY() + h));
        }
    }
    
    // Shuffle the vector
    std::shuffle(std::begin(eligeble_tiles), std::end(eligeble_tiles), RndManager::GetInstance().GetGenerator());
    
    auto can_place_stairs = [&] (Tile* tile) -> bool {
        assert (tile != nullptr);
        
        // Door and floor tiles are not eligible
        if (tile->HasTag(DOOR_TAG_) || !tile->HasTag(WALL_TAG_))
            return false;
        
        //    Get the four neighbours
        auto neis {map_->GetNeighbors(tile, MoveDirections::FOUR_DIRECTIONAL)};
        
        // Discard any near the border location
        if (neis.size() != 4)
            return false;
        
        auto wall_count {0};
        for (auto const &nei : neis) {
            
            // Neighboring tiles cannot have stairs
            if (nei->HasTag(UPSTAIRS_TAG_) || nei->HasTag(DOWNSTAIRS_TAG_))
                return false;
            
            // Count how many neighbours has walls
            if (nei->HasTag(WALL_TAG_))
                wall_count++;
        }
        
        // Wall embedded staris can only be placed into tiles adjacent to 3 walls, four directionally
        if (wall_count == 3)
            return true;
        
        return false;
    };
    
    auto iterate_and_place = [&] (size_t amount, bool is_upstair) {
        for (auto i {0}; i < amount; i++) {
            if (eligeble_tiles.size() == 0)
                break;
            
            if (auto tile {eligeble_tiles.back()}; can_place_stairs(tile)) {
                PlaceStairs(tile, is_upstair);
            }
            else
                i--;
            
            eligeble_tiles.pop_back();
        }
    };
    
    iterate_and_place(up, true);
    iterate_and_place(down, false);
}
    
void DungeonBuilder::GenerateGroundStairs(size_t up, size_t down, bool only_in_rooms, bool dig_space) {
    auto dungeon_map {(DungeonMap*)map_.get()};

    if (dungeon_map->GetRoomList().empty() || map_->GetMap()->empty()) {
        Utils::LogWarning("DungeonBuilder::GenerateGroundStairs", "There are no rooms, or no free space. Skipping door generation...");
        return;
    }
    
    std::vector<Tile*> eligeble_tiles;
    
    if (only_in_rooms) {
        for (auto const &room : dungeon_map->GetRoomList()) {
            // Scan the rooms and adds tiles
            for (auto w {room->GetRect().GetX()}; w < room->GetRect().GetX() + room->GetRect().GetWidth(); w++) {
                for (auto h {room->GetRect().GetY()}; h < room->GetRect().GetY() + room->GetRect().GetHeight(); h++) {
                    if (auto tile {map_->GetTile(w, h)}; !tile->HasAnyTag({DOWNSTAIRS_TAG_, UPSTAIRS_TAG_, DOOR_TAG_, WALL_TAG_}))
                        eligeble_tiles.push_back(tile);
                }
            }
        }
    } else {
        // Scan for walkable tiles
        for (auto const &tile : *map_->GetMap()) {
            if (!tile->HasAnyTag({DOWNSTAIRS_TAG_, UPSTAIRS_TAG_, DOOR_TAG_, WALL_TAG_})) {
                eligeble_tiles.push_back(tile.get());
            }
        }
    }
    
    // Shuffle the vector
    std::shuffle(std::begin(eligeble_tiles), std::end(eligeble_tiles), RndManager::GetInstance().GetGenerator());
    
    auto can_place_stairs = [&] (Tile* tile) -> bool {
        assert (tile != nullptr);
                
        //    Get the four neighbours
        for (auto const &nei : map_->GetNeighbors(tile, MoveDirections::EIGHT_DIRECTIONAL)) {
            // Neighboring tiles cannot have stairs
            if (nei->HasAnyTag({DOOR_TAG_, UPSTAIRS_TAG_, DOWNSTAIRS_TAG_}))
                return false;
        }
        
        return true;
    };
    
    auto iterate_and_place = [&] (size_t amount, bool is_upstair) {
        for (auto i {0}; i < amount; i++) {
            if (eligeble_tiles.size() == 0)
                break;
            
            if (auto tile {eligeble_tiles.back()}; can_place_stairs(tile)) {
                PlaceStairs(tile, is_upstair);
                
                if (dig_space) {
                    // Remove walls from neighbors
                    for (auto const &nei : map_->GetNeighbors(tile, MoveDirections::EIGHT_DIRECTIONAL)) {
                        // Neighboring tiles cannot have stairs
                        nei->RemoveTag(WALL_TAG_);
                    }
                }
            }
            else
                i--;
            
            eligeble_tiles.pop_back();
        }
    };
    
    iterate_and_place(up, true);
    iterate_and_place(down, false);
}

void DungeonBuilder::PlaceRect(Rect const &rect, std::initializer_list<Tag_p> tags) {
    for (auto i {rect.GetY()}; i < rect.GetY() + rect.GetHeight(); i++) {
        for (auto j {rect.GetX()}; j < rect.GetX() + rect.GetWidth(); j++) {
            if (map_->GetTile(j, i) != nullptr)
                map_->GetTile(j, i)->AddTags(tags);
            else break;
        }
    }
}

void DungeonBuilder::PlaceRoom(std::unique_ptr<Room> &room) {
    if (map_->GetMap()->empty()) {
        Utils::LogWarning("DungeonBuilder::placeRoom", "map_ has not been not initialized.\nInitializing now...");
        InitMap();
    }
    
    auto dungeon_map {(DungeonMap*)map_.get()};
    
    UpdateRect(room->GetRect(),
               {FLOOR_TAG_},
               {WALL_TAG_});
        dungeon_map->GetRoomList().push_back(std::move(room));
    dungeon_map->GetRoomList().back()->Print();
}

void DungeonBuilder::RemoveRect(Rect const &rect, std::initializer_list<Tag_p> tags) {
    for (auto i {rect.GetY()}; i < rect.GetY() + rect.GetHeight(); i++) {
        for (auto j {rect.GetX()}; j < rect.GetX() + rect.GetWidth(); j++) {
            if (map_->GetTile(j, i) != nullptr)
                map_->GetTile(j, i)->RemoveTags(tags);
        }
    }
}

bool DungeonBuilder::CanPlaceRect(Rect const &rect,
                              std::initializer_list<Tag_p> black_list,
                              std::initializer_list<Tag_p> white_list) {
    if (rect.GetHeight() == 0 || rect.GetWidth() == 0)
        return false;
    
    for (auto i { rect.GetY()}; i < rect.GetY() + rect.GetHeight(); i++) {
        for (auto j {rect.GetX()}; j < rect.GetX() + rect.GetWidth(); j++) {
            if (map_->GetTile(j, i) == nullptr
                || (map_->GetTile(j, i)->HasAnyTag(black_list)
                    && !map_->GetTile(j, i)->HasAnyTag(white_list)))
                return false;
        }
    }
    
    return true;
}

void DungeonBuilder::SetMaxRoomPlacementAttempts(size_t attempts) {
    assert(attempts != 0 && map_->GetMap()->empty());
    
    ((DungeonMapConfigs&)map_->GetConfigs()).max_room_placement_attempts_ = attempts;
}

void DungeonBuilder::SetMaxRoomSize(size_t width, size_t height) {
    auto dungeon_configs {(DungeonMapConfigs&)map_->GetConfigs()};
    
    assert(width < dungeon_configs.map_width_
           && height < dungeon_configs.map_height_
           && width >= dungeon_configs.min_room_width_
           && height >= dungeon_configs.min_room_height_
           && map_->GetMap()->empty());
    
    dungeon_configs.max_room_width_ = width;
    dungeon_configs.max_room_height_ = height;
}

void DungeonBuilder::SetMinRoomSize(size_t width, size_t height) {
    auto dungeon_configs {(DungeonMapConfigs&)map_->GetConfigs()};
    
    assert(width < dungeon_configs.map_width_
           && height < dungeon_configs.map_height_
           && width >= dungeon_configs.min_room_width_
           && height >= dungeon_configs.min_room_height_
           && map_->GetMap()->empty());

    dungeon_configs.min_room_width_ = width;
    dungeon_configs.min_room_height_ = height;
}

void DungeonBuilder::UpdateRect(Rect const &rect,
                            std::initializer_list<Tag_p> to_insert,
                            std::initializer_list<Tag_p> to_remove) {
    this->PlaceRect(rect, to_insert);
    this->RemoveRect(rect, to_remove);
}
    
}
