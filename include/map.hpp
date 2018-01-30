#ifndef LIBPMG_MAP_HPP_
#define LIBPMG_MAP_HPP_

#include <memory>
#include <tuple>

#include "grid.hpp"
#include "map_configs.hpp"
#include "room.hpp"
#include "tile.hpp"

namespace libpmg {
    
class Map : public Grid {
    friend class MapBuilder;
    
public:
    Map();
    Map(std::shared_ptr<Map> map);
    
    void Print();
    void ResetLocationCosts();
    void ResetPathFlags();
    
    std::shared_ptr<Tile> GetTile(std::size_t x, std::size_t y);
    std::shared_ptr<Tile> GetTile(std::pair<std::size_t, std::size_t> xy);
    std::vector<std::shared_ptr<Location>> GetNeighbors(std::shared_ptr<Location> location, MoveDirections dir = MoveDirections::FOUR_DIRECTIONAL);
    std::pair<std::size_t, std::size_t> GetMapSize();
    
    std::vector<Room> room_list;
    
protected:
    std::vector<std::shared_ptr<Tile>> map_;
    
    std::shared_ptr<MapConfigs> configs_;
    
    std::string map_uuid_;
    
    inline bool BoundsCheck(std::size_t x, std::size_t y) { return x < configs_->map_width_ && y < configs_->map_height_; }
    inline bool BoundsCheck(std::shared_ptr<Tile> tile) { return BoundsCheck(tile->GetX(), tile->GetY()); }    
};
    
}

#endif /* LIBPMG_MAP_HPP_ */
