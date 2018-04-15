#include "map.hpp"

#include "constants.hpp"
#include "utils.hpp"

namespace libpmg {

typedef std::shared_ptr<Location> Location_p;
typedef std::shared_ptr<Tile> Tile_p;
    
Map::Map() {
    map_uuid_ = Utils::GenerateUUID();
}

std::pair<size_t, size_t> Map::GetMapSize() {
    return std::make_pair(GetConfigs()->map_width_, GetConfigs()->map_height_);
}

std::vector<Location_p> Map::GetNeighbors(Location_p location, MoveDirections dir) {
    size_t x, y;
    std::tie(x, y) = location->GetXY();
    std::vector<Location_p> vec;
    
    if (auto tile {GetTile(x, y-1)}; tile != nullptr)
        vec.push_back(tile);
    if (auto tile {GetTile(x+1, y)}; tile != nullptr)
        vec.push_back(tile);
    if (auto tile {GetTile(x, y+1)}; tile != nullptr)
        vec.push_back(tile);
    if (auto tile {GetTile(x-1, y)}; tile != nullptr)
        vec.push_back(tile);
    
    if (dir == MoveDirections::EIGHT_DIRECTIONAL) {
        if (auto tile {GetTile(x-1, y-1)}; tile != nullptr)
            vec.push_back(tile);
        if (auto tile {GetTile(x+1, y+1)}; tile != nullptr)
            vec.push_back(tile);
        if (auto tile {GetTile(x-1, y+1)}; tile != nullptr)
            vec.push_back(tile);
        if (auto tile {GetTile(x+1, y-1)}; tile != nullptr)
            vec.push_back(tile);
        
    }
    
    return vec;
}

Tile_p Map::GetTile(std::pair<size_t, size_t> xy) {
    size_t x, y;
    std::tie(x, y) = xy;
    
    return GetTile (x, y);
}

Tile_p Map::GetTile(size_t x, size_t y) {
    if (!BoundsCheck(x, y))
        return nullptr;
    
    auto t {GetMap()[y * GetConfigs()->map_width_ + x]};
    
    return t;
}

void Map::Print() {
    std::string output {"\n"};
    
    for ( size_t i {0}; i < GetConfigs()->map_height_; i++ ) {
        for ( size_t j {0}; j < GetConfigs()->map_width_; j++ ) {
            if (auto t {GetTile(j, i)}; t != nullptr)
                output += t->GetChar();
        }
        output += '\n';
    }
    
    Utils::LogDebug("Map", output);
}
    
void Map::ResetLocationCosts() {
    for (auto const &tile : GetMap())
        tile->path_cost_ = kDefaultEmptyTileCost;
}

void Map::ResetPathFlags() {
    for (auto const &tile : GetMap()) tile->is_path_explored_ = false;
}
    
}
