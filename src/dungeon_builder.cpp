#include "dungeon_builder.hpp"

#include <cassert>
#include <queue>

#include "constants.hpp"
#include "dungeon_map.hpp"
#include "rnd_manager.hpp"
#include "utils.hpp"

namespace libpmg {
    
typedef std::shared_ptr<Tag> Tag_p;
typedef std::shared_ptr<std::unordered_map<Location*, Location*>> LocationMap_p;

DungeonBuilder::DungeonBuilder()
: default_path_algorithm_ {PathAlgorithm::ASTAR_BFS_MIX},
allow_diagonal_corridors_ {true} {
    map_ = std::make_shared<DungeonMap>();
}

std::shared_ptr<Map> DungeonBuilder::Build() {
    if (map_->map_.empty()) {
        Utils::LogError("DungeonBuilder::Build", "Map has not been not initialized.\nAborting...");
        abort();
    }
        
    return std::static_pointer_cast<Map> (map_);
}

void DungeonBuilder::SetDefaultPathAlgorithm(PathAlgorithm const &algorithm) {
    assert (map_->map_.empty());

    default_path_algorithm_ = algorithm;
}

void DungeonBuilder::SetDiagonalCorridors(bool allow) {
    assert (map_->map_.empty());

    allow_diagonal_corridors_ = allow;
}

void DungeonBuilder::SetMapSize(size_t width, size_t height) {
    assert (map_->map_.empty());

    map_->configs_->map_width_ = width;
    map_->configs_->map_height_ = height;
}

void DungeonBuilder::SetMaxRooms(size_t rooms) {
    assert (map_->map_.empty());

    map_->configs_->rooms_ = rooms;
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
    Location* start {map_->GetTile(room1.GetRndCoords())};
    Location* end {map_->GetTile(room2.GetRndCoords())};
    
    LocationMap_p path {nullptr};
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
    auto calculate_from_where = [=] (Location *coords, LocationMap_p came_from) -> Location* {
        for (auto const &kv : *came_from) {
            if (kv.first == coords)
                return kv.second;
        }
        
        Utils::LogError("Astar", "Broken path");
        abort();
    };
    
    // Flags the generated corridor with the proper tags
    while (end != start) {
        map_->GetTile(end->GetXY())->UpdateTags({TagManager::GetInstance().floor_tag_}, {TagManager::GetInstance().wall_tag_});
        end = calculate_from_where(end, path);
    }
    
    // Applies a cost to every tile in a room or a corridor, and to their neighbors, in order to
    // avoid corridors intersecating too much
    for (auto const &tile : map_->map_){
        if (tile->Taggable::HasTag(TagManager::GetInstance().floor_tag_)) {
            tile->path_cost_ = kDefaultWallTileCost;
            for (auto const &nei : map_->GetNeighbors(tile, MoveDirections::EIGHT_DIRECTIONAL))
                nei->path_cost_ = kDefaultWallTileCost;
        }
    }
}

void DungeonBuilder::GenerateCorridors() {
    if (map_->room_list_.empty() || map_->map_.empty()) {
        Utils::LogWarning("DungeonBuilder::placeCorridors", "There are no rooms, or no free space. Skipping corridor generation...");
        return;
    }
    
    for (auto i {0}; i < map_->room_list_.size() - 1; i++)
        ConnectRooms(*map_->room_list_.at(i), *map_->room_list_.at(i + 1));
    
    // In case of odd rooms
    if (map_->room_list_.size()%2 != 0)
        ConnectRooms(
                     *map_->room_list_.at(map_->room_list_.size()-2),
                     *map_->room_list_.at(map_->room_list_.size()-1));
}

void DungeonBuilder::InitMap() {
    for (auto i {0}; i < map_->configs_->map_height_; i++) {
        for (auto j {0}; j < map_->configs_->map_width_; j++)
            map_->map_.push_back(new Tile (j, i, {TagManager::GetInstance().wall_tag_}));
    }
}

void DungeonBuilder::ResetMap(bool keep_configs) {
    auto configs {map_->GetConfigs()};
    map_ = std::make_shared<DungeonMap>(configs);
    this->InitMap();
}

void DungeonBuilder::GenerateRooms() {
    if (map_->map_.empty()) {
        Utils::LogWarning("DungeonBuilder::GenerateRooms", "m_map has not been not initialized. Initializing now...");
        InitMap();
    }
    
    //  Generate each room.
    for (auto i {0}; i < map_->configs_->rooms_; i++) {
        for (auto j {0}; j < map_->configs_->max_room_placement_attempts_; j++) {
            
            //          Get random rect
            auto rndRect {Rect::GetRndRect(1, map_->configs_->map_width_-1,
                                           1, map_->configs_->map_height_-1,
                                           map_->configs_->min_room_width_, map_->configs_->max_room_width_,
                                           map_->configs_->min_room_height_, map_->configs_->max_room_height_)};
            
            if (CanPlaceRect(++rndRect, {TagManager::GetInstance().floor_tag_})) {
                PlaceRoom(new Room(--rndRect));
                break;
            }
            
            if (j == map_->configs_->max_room_placement_attempts_ - 1)
                Utils::LogDebug( "DungeonBuilder", "Last room placement attempt failed. Moving on...");
        }
    }
}

void DungeonBuilder::PlaceDoor(Tile *tile) {
    assert (tile != nullptr);
    
    if (!tile->HasTag(TagManager::GetInstance().floor_tag_))
        return;
    
    //    Get the four neighbours
    auto neis {map_->GetNeighbors(tile, MoveDirections::FOUR_DIRECTIONAL)};
    
    if (neis.size() < 4)
        return;
    
    auto wall_count {0};
    for (auto const &nei : neis) {
        // If one neighbour has a door, exit
        if (nei->HasTag(TagManager::GetInstance().door_tag_))
            return;
        
        // Count how many neighbours has walls
        if (nei->HasTag(TagManager::GetInstance().wall_tag_))
            wall_count++;
    }
    
    // If more then 2 neighbours has doors, there can't be two opposing wall tiles, therefore no place for a door
    if (wall_count >= 3)
        return;
    
    // Checks if the 2 neighbouring wall tiles are opposed to eachother
    if ((map_->GetTile(tile->GetX(), tile->GetY()-1)->HasTag(TagManager::GetInstance().wall_tag_)
         && map_->GetTile(tile->GetX(), tile->GetY()+1)->HasTag(TagManager::GetInstance().wall_tag_))
        || (map_->GetTile(tile->GetX()-1, tile->GetY())->HasTag(TagManager::GetInstance().wall_tag_)
            && map_->GetTile(tile->GetX()+1, tile->GetY())->HasTag(TagManager::GetInstance().wall_tag_)))
        tile->AddTag(TagManager::GetInstance().door_tag_);
}
    
bool DungeonBuilder::PlaceStairs(Tile *tile, bool is_upstairs) {
    assert (tile != nullptr);
    
    // Door and floor tiles are not eligible
    if (tile->HasTag(TagManager::GetInstance().door_tag_) || !tile->HasTag(TagManager::GetInstance().wall_tag_))
        return false;
    
    //    Get the four neighbours
    auto neis {map_->GetNeighbors(tile, MoveDirections::FOUR_DIRECTIONAL)};
    
    // Discard any near the border location
    if (neis.size() != 4)
        return false;
        
    auto wall_count {0};
    for (auto const &nei : neis) {
        
        // Neighboring tiles cannot have stairs
        if (nei->HasTag(TagManager::GetInstance().upstairs_tag_) || nei->HasTag(TagManager::GetInstance().downstairs_tag_))
            return false;
            
        // Count how many neighbours has walls
        if (nei->HasTag(TagManager::GetInstance().wall_tag_))
            wall_count++;
    }
    
    // Wall embedded staris can only be placed into tiles adjacent to 3 walls, four directionally
    if (wall_count == 3) {
        if (is_upstairs)
            tile->UpdateTags({TagManager::GetInstance().upstairs_tag_}, {TagManager::GetInstance().wall_tag_});
        else
            tile->UpdateTags({TagManager::GetInstance().downstairs_tag_}, {TagManager::GetInstance().wall_tag_});

        return true;
    }
    
    return false;
}

void DungeonBuilder::GenerateDoors() {
    if (map_->room_list_.empty() || map_->map_.empty()) {
        Utils::LogWarning("DungeonBuilder::GenerateDoors", "There are no rooms, or no free space. Skipping door generation...");
        return;
    }
    
    // Scan the borders fo the rooms form tiles eligible for doors
    for (auto const &room : map_->room_list_) {
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
    if (map_->room_list_.empty() || map_->map_.empty()) {
        Utils::LogWarning("DungeonBuilder::GenerateWallStairs", "There are no rooms, or no free space. Skipping door generation...");
        return;
    }
    
    std::vector<Tile*> eligeble_tiles;

    // Scan the borders fo the rooms form tiles eligible for stairs
    for (auto const &room : map_->room_list_) {
        Rect rect {room->Area::GetRect()};
        rect++;

        for (auto w {0}; w < rect.GetWidth(); w++) {
            eligeble_tiles.push_back(map_->GetTile(rect.GetX() + w, rect.GetY()));

            eligeble_tiles.push_back(map_->GetTile(rect.GetX() + w, rect.GetY() + rect.GetHeight()-1));
        }
        
        for (auto h {0}; h < rect.GetHeight(); h++) {
            eligeble_tiles.push_back(map_->GetTile(rect.GetX(), rect.GetY() + h));

            eligeble_tiles.push_back(map_->GetTile(rect.GetX() + rect.GetWidth()-1, rect.GetY() + h));
        }
    }
    
    // Shuffle the vector
    std::shuffle(std::begin(eligeble_tiles), std::begin(eligeble_tiles), *RndManager::GetInstance().GetGenerator());
    
    
    // TODO:
//    remove stares from angles.
    
    for (auto const &tile : eligeble_tiles) {
        PlaceStairs(tile, true);
    }
}
    
void DungeonBuilder::GenerateGroundStairs(size_t up, size_t down) {
    if (map_->room_list_.empty() || map_->map_.empty()) {
        Utils::LogWarning("DungeonBuilder::GenerateGroundStairs", "There are no rooms, or no free space. Skipping door generation...");
        return;
    }
    
    std::vector<Tile*> eligeble_tiles;
    
//    std::shuffle(eligeble_tiles.front(), eligeble_tiles.back(), *RndManager::GetInstance().GetGenerator());
    
    for (auto const &tile : eligeble_tiles) {
//        tile->UpdateTags({TagManager::GetInstance().stairs_tag_}, {TagManager::GetInstance().wall_tag_});
    }
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

void DungeonBuilder::PlaceRoom(Room *room) {
    if (map_->map_.empty()) {
        Utils::LogWarning("DungeonBuilder::placeRoom", "map_ has not been not initialized.\nInitializing now...");
        InitMap();
    }
    
    UpdateRect(room->GetRect(),
               {TagManager::GetInstance().floor_tag_},
               {TagManager::GetInstance().wall_tag_});
    map_->room_list_.push_back(room);
    room->Print();
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
    assert(attempts != 0 && map_->map_.empty());
    
    map_->configs_->max_room_placement_attempts_ = attempts;
}

void DungeonBuilder::SetMaxRoomSize(size_t width, size_t height) {
    assert(width < map_->configs_->map_width_
           && height < map_->configs_->map_height_
           && width >= map_->configs_->min_room_width_
           && height >= map_->configs_->min_room_height_
           && map_->map_.empty());
    
    map_->configs_->max_room_width_ = width;
    map_->configs_->max_room_height_ = height;
}

void DungeonBuilder::SetMinRoomSize(size_t width, size_t height) {
    assert(width < map_->configs_->map_width_
           && height < map_->configs_->map_height_
           && width >= map_->configs_->min_room_width_
           && height >= map_->configs_->min_room_height_
           && map_->map_.empty());
    
    map_->configs_->min_room_width_ = width;
    map_->configs_->min_room_height_ = height;
}

void DungeonBuilder::UpdateRect(Rect const &rect,
                            std::initializer_list<Tag_p> to_insert,
                            std::initializer_list<Tag_p> to_remove) {
    this->PlaceRect(rect, to_insert);
    this->RemoveRect(rect, to_remove);
}
    
}
