/**
 @file world_tile.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_WORLD_TILE_HPP_
#define LIBPMG_WORLD_TILE_HPP_

namespace libpmg {
    
/**
 Represent every biome available for a world map tile.
 */
enum struct BiomeType {
    DEEP_SEA,
    HIGH_SEA,
    SHALLOW_SEA,
    DESERT,
    GRASSLAND,
    FOREST,
    JUNGLE,
    INLAND_WATER,
    HILL,
    ROCK_MOUNTAIN,
    SNOW
};
    
/**
 This class holds the extra information needed by world map tile
 */
class WorldTile : public Tile {
    
public:
    friend class WorldBuilder;
    
    WorldTile (std::size_t x, std::size_t y) : Tile (x, y) {}
    
    inline float GetAltitude() { return altitude_; }
    
private:
    float altitude_, temperature_;
    BiomeType biome_;
};
    
}

#endif /* LIBPMG_WORLD_TILE_HPP_ */
