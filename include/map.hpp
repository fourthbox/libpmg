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
    Tile *GetTile(std::size_t x, std::size_t y);
    
    /**
     Get a tile in a specified location.
     @param xy A pair containing the coordinates
     @return A pointer to the tile
     */
    Tile *GetTile(std::pair<std::size_t, std::size_t> xy);
    
    /**
     Gets the tiles adjacent to the selected location.
     @param location The location to get the neighbors from
     @param dir Whether getting only tiles adjacent on cardinal directions, or diagonal tiles
     @return A vector of pointers to the neighbor locations
     */
    std::vector<Location*> GetNeighbors(Location *location, MoveDirections const &dir = MoveDirections::FOUR_DIRECTIONAL) override;
    
    /**
     Gets the tiles adjacent to the selected tile.
     @param location The tile to get the neighbors from
     @param dir Whether getting only tiles adjacent on cardinal directions, or diagonal tiles
     @return A vector of pointers to the neighbor tiles
     */
    std::vector<Tile*> GetNeighbors(Tile *location, MoveDirections const &dir = MoveDirections::FOUR_DIRECTIONAL);

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
     @return A reference to the configuration MapConfigs for this map.
     */
    virtual MapConfigs &GetConfigs() = 0;
    
    /**
     Gets the map.
     @return A pointer to the vector containing all the Tile in this map
     */
    virtual std::unique_ptr<std::vector<std::unique_ptr<Tile>>> &GetMap() = 0;
    
    /**
     Checks whether the specified coordinates are inside of the map.
     @param x The X coordinate
     @param y The Y coordinate
     @return True if the coordinate are inside the map, false otherwise.
     */
    inline bool BoundsCheck(std::size_t x, std::size_t y) { return x < GetConfigs().map_width_ && y < GetConfigs().map_height_; }
    
    /**
     Checks whether the specified tile is inside of the map.
     @param tile A pointer to the tile to check
     @return True if the tile is inside the map, false otherwise.
     */
    constexpr bool BoundsCheck(Tile const &tile) { return BoundsCheck(tile.GetX(), tile.GetY()); }

};
    
inline Map::~Map() {}
    
}

#endif /* LIBPMG_MAP_HPP_ */
