#include "map_builder.hpp"

#include <cassert>

#include "rnd_manager.hpp"
#include "utils.hpp"

namespace libpmg {

typedef std::shared_ptr<Location> Location_p;
typedef std::shared_ptr<std::unordered_map<Location_p, Location_p>> LocationMap_p;
typedef std::shared_ptr<Map> Map_p;
typedef std::shared_ptr<Tag> Tag_p;
typedef std::shared_ptr<Tile> Tile_p;

MapBuilder::MapBuilder()
: default_path_algorithm_ {PathAlgorithm::ASTAR_BFS_MIX},
allow_diagonal_corridors_ {true} {
    map_ = std::make_shared<Map>(Map());
}

Map_p MapBuilder::Build() {
    Utils::LogDebug("Seed", std::to_string(RndManager::seed));
    
    if (map_->map_.empty()) {
        Utils::LogWarning("MapBuilder::Build", "Map has not been not initialized.\nAborting...");
        abort();
    }
    
    return map_;
}

MapBuilder& MapBuilder::SetDefaultPathAlgorithm(PathAlgorithm algorithm) {
    default_path_algorithm_ = algorithm;
    return *this;
}

MapBuilder& MapBuilder::SetDiagonalCorridors(bool allow) {
    allow_diagonal_corridors_ = allow;
    return *this;
}

MapBuilder& MapBuilder::SetMapSize(size_t width, size_t height) {
    map_->configs_->map_width_ = width;
    map_->configs_->map_height_ = height;
    return *this;
}

MapBuilder& MapBuilder::SetMaxRooms(size_t rooms) {
    map_->configs_->rooms_ = rooms;
    return *this;
}

// Returns the Location from which come from coords
Location_p from(Location_p coords, LocationMap_p came_from) {
    for (auto const &kv : *came_from) {
        if (kv.first == coords)
            return kv.second;
    }
    
    Utils::LogError("Astar", "Broken path");
    abort();
}

bool MapBuilder::IsDiagonalCorridor() {
    if (allow_diagonal_corridors_)
        return RndManager::GetInstance().GetRandomUintFromRange(0,1);
    
    return false;
}

/**
 Connects two rooms with a corridor, generated with a random combination of Astar and BreadthFirstSearch.
 
 @param room1 The first room.
 @param room2 The second room.
 */
void MapBuilder::ConnectRooms(Room room1, Room room2) {
    Location_p start {map_->GetTile(room1.GetRndCoords())};
    Location_p end {map_->GetTile(room2.GetRndCoords())};
    
    LocationMap_p path {nullptr};
    switch (default_path_algorithm_) {
        case PathAlgorithm::BREADTH_FIRST_SEARCH:
            path = Utils::BreadthFirstSearch(
                                             start->GetXY(),
                                             end->GetXY(),
                                             map_,
                                             IsDiagonalCorridor(),
                                             MoveDirections::FOUR_DIRECTIONAL);
            break;
        case PathAlgorithm::DIJKSTRA:
            path = Utils::Dijkstra(
                                   start->GetXY(),
                                   end->GetXY(),
                                   map_,
                                   MoveDirections::FOUR_DIRECTIONAL);
            break;
        case PathAlgorithm::ASTAR:
            path = Utils::Astar(
                                start->GetXY(),
                                end->GetXY(),
                                map_,
                                MoveDirections::FOUR_DIRECTIONAL);
            break;
        case PathAlgorithm::ASTAR_BFS_MIX:
        default:
            if (RndManager::GetInstance().GetRandomUintFromRange(0,1))
                path = Utils::Astar(
                                    start->GetXY(),
                                    end->GetXY(),
                                    map_,
                                    MoveDirections::FOUR_DIRECTIONAL);
            else
                path = Utils::BreadthFirstSearch(
                                                 start->GetXY(),
                                                 end->GetXY(),
                                                 map_,
                                                 IsDiagonalCorridor(),
                                                 MoveDirections::FOUR_DIRECTIONAL);
            break;
    }
    
    assert(path != nullptr);
    
    // Flags the generated corridor with the proper tags
    while (end != start) {
        map_->GetTile(end->GetXY())->UpdateTags({TagManager::GetInstance().floor_tag}, {TagManager::GetInstance().wall_tag});
        end = from(end, path);
    }
    
    // Applies a cost to every tile in a room or a corridor, and to their neighbors, in order to
    // avoid corridors intersecating too much
    auto wall_cost {kDefaultWallTileCost};
    for (auto const &tile : map_->map_){
        if (tile->Taggable::HasTag(TagManager::GetInstance().floor_tag)) {
            tile->cost = wall_cost;
            for (auto const &nei : map_->GetNeighbors(tile, MoveDirections::EIGHT_DIRECTIONAL))
                nei->cost = wall_cost;
        }
    }
}

MapBuilder& MapBuilder::GenerateCorridors() {
    if (map_->room_list.empty() || map_->map_.empty()) {
        Utils::LogWarning("MapBuilder::placeCorridors", "There are no rooms, or no free space. Skipping corridor generation...");
        return *this;
    }
    
    for (auto i {0}; i < map_->room_list.size() - 1; i++)
        ConnectRooms(map_->room_list.at(i), map_->room_list.at(i + 1));
    
    // In case of odd rooms
    if (map_->room_list.size()%2 != 0)
        ConnectRooms(
                     map_->room_list.at(map_->room_list.size()-2),
                     map_->room_list.at(map_->room_list.size()-1));
    
    return *this;
}

MapBuilder& MapBuilder::InitMap() {
    for (auto i {0}; i < map_->configs_->map_height_; i++) {
        for (auto j {0}; j < map_->configs_->map_width_; j++)
            map_->map_.push_back(std::make_shared<Tile>(Tile (j, i, {TagManager::GetInstance().wall_tag})));
    }
    
    return *this;
}

MapBuilder& MapBuilder::ResetMap() {
    this->InitMap();
    return *this;
}

MapBuilder& MapBuilder::GenerateRooms() {
    if (map_->map_.empty()) {
        Utils::LogWarning("MapBuilder::GenerateRooms", "m_map has not been not initialized. Initializing now...");
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
            
            if (CanPlaceRect(++rndRect, {TagManager::GetInstance().floor_tag})) {
                PlaceRoom(Room(--rndRect));
                break;
            }
            
            if (j == map_->configs_->max_room_placement_attempts_ - 1)
                Utils::LogDebug( "MapBuilder", "Last room placement attempt failed. Moving on...");
        }
    }
    
    return *this;
}

/**
 Place a door if the tile is eligible.
 In order be aligible for a door a tile must be adjacent to no more then two wall tiles, and they must be opposite to eachother.
 
 @param tile The tile that will host the door.
 */
void MapBuilder::PlaceDoor(Tile_p tile) {
    assert (tile != nullptr);
    
    if (!tile->HasTag(TagManager::GetInstance().floor_tag))
        return;
    
    //    Get the four neighbours
    auto neis {map_->GetNeighbors(tile, MoveDirections::FOUR_DIRECTIONAL)};
    
    if (neis.size() < 4)
        return;
    
    auto wall_count {0};
    for (auto const &nei : neis) {
        // If one neighbour has a door, exit
        if (map_->GetTile(nei->GetXY())->HasTag(TagManager::GetInstance().door_tag))
            return;
        
        // Count how many neighbours has walls
        if (map_->GetTile(nei->GetXY())->HasTag(TagManager::GetInstance().wall_tag))
            wall_count++;
    }
    
    // If more then 2 neighbours has doors, there can't be two opposing wall tiles, therefore no place for a door
    if (wall_count >= 3)
        return;
    
    // Checks if the 2 neighbouring wall tiles are opposed to eachother
    if ((map_->GetTile(tile->GetX(), tile->GetY()-1)->HasTag(TagManager::GetInstance().wall_tag)
         && map_->GetTile(tile->GetX(), tile->GetY()+1)->HasTag(TagManager::GetInstance().wall_tag))
        || (map_->GetTile(tile->GetX()-1, tile->GetY())->HasTag(TagManager::GetInstance().wall_tag)
            && map_->GetTile(tile->GetX()+1, tile->GetY())->HasTag(TagManager::GetInstance().wall_tag)))
        tile->AddTag(TagManager::GetInstance().door_tag);
}

MapBuilder& MapBuilder::GenerateDoors() {
    if (map_->room_list.empty() || map_->map_.empty()) {
        Utils::LogWarning("MapBuilder::placeCorridors", "There are no rooms, or no free space. Skipping door generation...");
        return *this;
    }
    
    for (auto &room : map_->room_list) {
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
    
    return *this;
}

void MapBuilder::PlaceRect(Rect rect, std::initializer_list<Tag_p> tags) {
    for (auto i {rect.GetY()}; i < rect.GetY() + rect.GetHeight(); i++) {
        for (auto j {rect.GetX()}; j < rect.GetX() + rect.GetWidth(); j++) {
            if (map_->GetTile(j, i) != nullptr)
                map_->GetTile(j, i)->AddTags(tags);
            else break;
        }
    }
}

MapBuilder& MapBuilder::PlaceRoom(Room room) {
    if (map_->map_.empty()) {
        Utils::LogWarning("MapBuilder::placeRoom", "map_ has not been not initialized.\nInitializing now...");
        InitMap();
    }
    
    UpdateRect(room.GetRect(),
               {TagManager::GetInstance().floor_tag},
               {TagManager::GetInstance().wall_tag});
    map_->room_list.push_back(room);
    room.Print();
    
    return *this;
}

void MapBuilder::RemoveRect(Rect rect, std::initializer_list<Tag_p> tags) {
    for (auto i {rect.GetY()}; i < rect.GetY() + rect.GetHeight(); i++) {
        for (auto j {rect.GetX()}; j < rect.GetX() + rect.GetWidth(); j++) {
            if (map_->GetTile(j, i) != nullptr)
                map_->GetTile(j, i)->RemoveTags(tags);
        }
    }
}

bool MapBuilder::CanPlaceRect(Rect rect,
                              std::initializer_list<Tag_p> black_list,
                              std::initializer_list<Tag_p> white_list) {
    if (rect.GetHeight() == 0 || rect.GetWidth() == 0)
        return false;
    
    for (auto i { rect.GetY()}; i < rect.GetY() + rect.GetHeight(); i++) {
        for (auto j {rect.GetX()}; j < rect.GetX() + rect.GetWidth(); j++) {
            if (map_->GetTile(j, i) == nullptr || map_->GetTile(j, i)->HasAnyTag(black_list))
                return false;
        }
    }
    
    return true;
}

MapBuilder& MapBuilder::SetMaxRoomPlacementAttempts(size_t attempts) {
    assert(attempts != 0);
    
    map_->configs_->max_room_placement_attempts_ = attempts;
    return *this;
}

MapBuilder& MapBuilder::SetMaxRoomSize(size_t width, size_t height) {
    assert(width < map_->configs_->map_width_
           && height < map_->configs_->map_height_
           && width >= map_->configs_->min_room_width_
           && height >= map_->configs_->min_room_height_);
    
    map_->configs_->max_room_width_ = width;
    map_->configs_->max_room_height_ = height;
    
    return *this;
}

MapBuilder& MapBuilder::SetMinRoomSize(size_t width, size_t height) {
    assert(width < map_->configs_->map_width_
           && height < map_->configs_->map_height_
           && width >= map_->configs_->min_room_width_
           && height >= map_->configs_->min_room_height_);
    
    map_->configs_->min_room_width_ = width;
    map_->configs_->min_room_height_ = height;
    
    return *this;
}

void MapBuilder::UpdateRect(Rect rect,
                            std::initializer_list<Tag_p> to_insert,
                            std::initializer_list<Tag_p> to_remove) {
    this->PlaceRect(rect, to_insert);
    this->RemoveRect(rect, to_remove);
}
    
}
