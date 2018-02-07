#ifndef LIBPMG_MAP_HPP_
#define LIBPMG_MAP_HPP_

#include "grid.hpp"
#include "tile.hpp"

namespace libpmg {
    
class MapConfigs {
public:
    MapConfigs()
    : map_width_ {185},
    map_height_ {45}
    {}
    
    std::size_t map_width_;
    std::size_t map_height_;
};
    
class Map : public Grid {
    friend class MapBuilder;
    
public:
    Map();
    virtual ~Map() = 0;
    
    void Print();
    
    std::shared_ptr<Tile> GetTile(std::size_t x, std::size_t y);
    std::shared_ptr<Tile> GetTile(std::pair<std::size_t, std::size_t> xy);
    std::vector<std::shared_ptr<Location>> GetNeighbors(std::shared_ptr<Location> location, MoveDirections dir = MoveDirections::FOUR_DIRECTIONAL) override;
    std::pair<std::size_t, std::size_t> GetMapSize();
    
    void ResetLocationCosts() override;
    void ResetPathFlags();
    
protected:    
    std::string map_uuid_;
    std::shared_ptr<MapConfigs> configs_;
    
    std::vector<std::shared_ptr<Tile>> map_;
    
    inline bool BoundsCheck(std::size_t x, std::size_t y) { return x < configs_->map_width_ && y < configs_->map_height_; }
    inline bool BoundsCheck(std::shared_ptr<Tile> tile) { return BoundsCheck(tile->GetX(), tile->GetY()); }

};
    
inline Map::~Map() {}
    
}

#endif /* LIBPMG_MAP_HPP_ */
