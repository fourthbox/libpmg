/**
 @file map.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_MAP_HPP_
#define LIBPMG_MAP_HPP_

#include "grid.hpp"
#include "tile.hpp"

namespace libpmg {
    
/**
 A struct that defines the configuration values for every map.
 */
struct MapConfigs {
    MapConfigs()
    : map_width_ {185},
    map_height_ {45}
    {}
    
    std::size_t map_width_;
    std::size_t map_height_;
};
    
/**
 A pure virtual class that represent a generic Map class.
 */
class Map : public Grid {
public:
    Map();
    virtual ~Map() = 0;
    
    /**
     Prints debug informations and a ASCII sketch of the map.
     */
    void Print();
    
    /**
     Get a tile in a specified location.
     @param x The X coordinate
     @param y The Y coordinate
     @return A pointer to the tile
     */
    std::shared_ptr<Tile> GetTile(std::size_t x, std::size_t y);
    
    /**
     Get a tile in a specified location.
     @param xy A pair containing the coordinates
     @return A pointer to the tile
     */
    std::shared_ptr<Tile> GetTile(std::pair<std::size_t, std::size_t> xy);
    
    /**
     Gets the tiles adjacent to the selected tile.
     @param location The tile to get the neighbors from
     @param dir Whether getting only tiles adjacent on cardinal directions, or diagonal tiles
     @return A vector of pointers to the neighbor tiles
     */
    std::vector<std::shared_ptr<Location>> GetNeighbors(std::shared_ptr<Location> location, MoveDirections dir = MoveDirections::FOUR_DIRECTIONAL) override;
    
    /**
     Get the map size.
     @return A pair containing map width and height
     */
    std::pair<std::size_t, std::size_t> GetMapSize();
    
    /**
     Resets all the costs calculated by the path finding algorithm.
     */
    void ResetLocationCosts() override;
    
    /**
     Resets all the flags added by the path finding algorithm.
     */
    void ResetPathFlags() override;
    
protected:    
    std::string map_uuid_;      /**< A unique id for this particular instance. */
    
    /**
     Gets the configuration MapConfigs for this map.
     @return A pointer to the configuration MapConfigs for this map.
     */
    virtual std::shared_ptr<MapConfigs> GetConfigs() = 0;
    
    /**
     Gets the map.
     @return A vector containing all the Tile in this map
     */
    virtual std::vector<std::shared_ptr<Tile>> &GetMap() = 0;
    
    /**
     Checks whether the specified coordinates are inside of the map.
     @param x The X coordinate
     @param y The Y coordinate
     @return True if the coordinate are inside the map, false otherwise.
     */
    inline bool BoundsCheck(std::size_t x, std::size_t y) { return x < GetConfigs()->map_width_ && y < GetConfigs()->map_height_; }
    
    /**
     Checks whether the specified tile is inside of the map.
     @param tile A pointer to the tile to check
     @return True if the tile is inside the map, false otherwise.
     */
    inline bool BoundsCheck(std::shared_ptr<Tile> tile) { return BoundsCheck(tile->GetX(), tile->GetY()); }

};
    
inline Map::~Map() {}
    
}

#endif /* LIBPMG_MAP_HPP_ */
