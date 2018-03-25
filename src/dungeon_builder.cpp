#include "dungeon_builder.hpp"

#include <cassert>
#include <queue>

#include "constants.hpp"
#include "dungeon_map.hpp"
#include "rnd_manager.hpp"
#include "utils.hpp"

namespace libpmg {
    
typedef std::shared_ptr<Location> Location_p;
typedef std::shared_ptr<std::unordered_map<Location_p, Location_p>> LocationMap_p;
typedef std::shared_ptr<Map> Map_p;
typedef std::shared_ptr<Tag> Tag_p;
typedef std::shared_ptr<Tile> Tile_p;

DungeonBuilder::DungeonBuilder()
: default_path_algorithm_ {PathAlgorithm::ASTAR_BFS_MIX},
allow_diagonal_corridors_ {true} {
    map_ = std::make_unique<DungeonMap>();
}

std::unique_ptr<Map> DungeonBuilder::Build() {
    if (map_->map_.empty()) {
        Utils::LogError("DungeonBuilder::Build", "Map has not been not initialized.\nAborting...");
        abort();
    }
        
    return std::move(map_);
}

void DungeonBuilder::SetDefaultPathAlgorithm(PathAlgorithm algorithm) {
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

// Returns the Location from which coords it come from
Location_p from(Location_p coords, LocationMap_p came_from) {
    for (auto const &kv : *came_from) {
        if (kv.first == coords)
            return kv.second;
    }
    
    Utils::LogError("Astar", "Broken path");
    abort();
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
void DungeonBuilder::ConnectRooms(Room room1, Room room2) {
    Location_p start {map_->GetTile(room1.GetRndCoords())};
    Location_p end {map_->GetTile(room2.GetRndCoords())};
    
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
    
    // Flags the generated corridor with the proper tags
    while (end != start) {
        map_->GetTile(end->GetXY())->UpdateTags({TagManager::GetInstance().floor_tag_}, {TagManager::GetInstance().wall_tag_});
        end = from(end, path);
    }
    
    // Applies a cost to every tile in a room or a corridor, and to their neighbors, in order to
    // avoid corridors intersecating too much
    for (auto const &tile : map_->map_){
        if (tile->Taggable::HasTag(TagManager::GetInstance().floor_tag_)) {
            tile->cost_ = kDefaultWallTileCost;
            for (auto const &nei : map_->GetNeighbors(tile, MoveDirections::EIGHT_DIRECTIONAL))
                nei->cost_ = kDefaultWallTileCost;
        }
    }
}

void DungeonBuilder::GenerateCorridors() {
    if (map_->room_list_.empty() || map_->map_.empty()) {
        Utils::LogWarning("DungeonBuilder::placeCorridors", "There are no rooms, or no free space. Skipping corridor generation...");
        return;
    }
    
    for (auto i {0}; i < map_->room_list_.size() - 1; i++)
        ConnectRooms(map_->room_list_.at(i), map_->room_list_.at(i + 1));
    
    // In case of odd rooms
    if (map_->room_list_.size()%2 != 0)
        ConnectRooms(
                     map_->room_list_.at(map_->room_list_.size()-2),
                     map_->room_list_.at(map_->room_list_.size()-1));
}

void DungeonBuilder::InitMap() {
    for (auto i {0}; i < map_->configs_->map_height_; i++) {
        for (auto j {0}; j < map_->configs_->map_width_; j++)
            map_->map_.push_back(std::make_shared<Tile>(Tile (j, i, {TagManager::GetInstance().wall_tag_})));
    }
}

void DungeonBuilder::ResetMap() {
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
                PlaceRoom(Room(--rndRect));
                break;
            }
            
            if (j == map_->configs_->max_room_placement_attempts_ - 1)
                Utils::LogDebug( "DungeonBuilder", "Last room placement attempt failed. Moving on...");
        }
    }
}

void DungeonBuilder::PlaceDoor(Tile_p tile) {
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
        if (map_->GetTile(nei->GetXY())->HasTag(TagManager::GetInstance().door_tag_))
            return;
        
        // Count how many neighbours has walls
        if (map_->GetTile(nei->GetXY())->HasTag(TagManager::GetInstance().wall_tag_))
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

void DungeonBuilder::GenerateDoors() {
    if (map_->room_list_.empty() || map_->map_.empty()) {
        Utils::LogWarning("DungeonBuilder::placeCorridors", "There are no rooms, or no free space. Skipping door generation...");
        return;
    }
    
    for (auto &room : map_->room_list_) {
        for (auto w {0}; w < (++room.Area::GetRect()).GetWidth(); w++) {
            auto tile {map_->GetTile((++room.Area::GetRect()).GetX() + w, (++room.Area::GetRect()).GetY())};
            PlaceDoor(tile);
            
            tile = map_->GetTile((++room.Area::GetRect()).GetX() + w, (++room.Area::GetRect()).GetY() + (++room.Area::GetRect()).GetHeight()-1);
            PlaceDoor(tile);
        }
        
        for (auto h {0}; h < (++room.Area::GetRect()).GetHeight(); h++) {
            auto tile {map_->GetTile((++room.Area::GetRect()).GetX(), (++room.Area::GetRect()).GetY() + h)};
            PlaceDoor(tile);
            
            tile = map_->GetTile((++room.Area::GetRect()).GetX() + (++room.Area::GetRect()).GetWidth()-1, (++room.Area::GetRect()).GetY() + h);
            PlaceDoor(tile);
        }
    }
}

void DungeonBuilder::PlaceRect(Rect rect, std::initializer_list<Tag_p> tags) {
    for (auto i {rect.GetY()}; i < rect.GetY() + rect.GetHeight(); i++) {
        for (auto j {rect.GetX()}; j < rect.GetX() + rect.GetWidth(); j++) {
            if (map_->GetTile(j, i) != nullptr)
                map_->GetTile(j, i)->AddTags(tags);
            else break;
        }
    }
}

void DungeonBuilder::PlaceRoom(Room room) {
    if (map_->map_.empty()) {
        Utils::LogWarning("DungeonBuilder::placeRoom", "map_ has not been not initialized.\nInitializing now...");
        InitMap();
    }
    
    UpdateRect(room.GetRect(),
               {TagManager::GetInstance().floor_tag_},
               {TagManager::GetInstance().wall_tag_});
    map_->room_list_.push_back(room);
    room.Print();
}

void DungeonBuilder::RemoveRect(Rect rect, std::initializer_list<Tag_p> tags) {
    for (auto i {rect.GetY()}; i < rect.GetY() + rect.GetHeight(); i++) {
        for (auto j {rect.GetX()}; j < rect.GetX() + rect.GetWidth(); j++) {
            if (map_->GetTile(j, i) != nullptr)
                map_->GetTile(j, i)->RemoveTags(tags);
        }
    }
}

bool DungeonBuilder::CanPlaceRect(Rect rect,
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

void DungeonBuilder::UpdateRect(Rect rect,
                            std::initializer_list<Tag_p> to_insert,
                            std::initializer_list<Tag_p> to_remove) {
    this->PlaceRect(rect, to_insert);
    this->RemoveRect(rect, to_remove);
}
    
}
