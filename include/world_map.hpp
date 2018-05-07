/**
 @file world_map.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_WORLD_MAP_HPP_
#define LIBPMG_WORLD_MAP_HPP_

#include "FastNoise.h"
#include "map.hpp"

namespace libpmg {
    
/**
 Struct that holds the DungeonMap configuration values
 */
struct WorldMapConfigs : public MapConfigs {
public:
    WorldMapConfigs()
    : noise_type_ {FastNoise::PerlinFractal},
    noise_frequency_ {0.005f},
    fractal_lacunarity_ {2.9f},
    fractal_gain_ {0.35f},
    fractal_octaves_ {10},
    extreme_multiplier_ {2.0f},
    sea_level_multiplier_ {2.5f},
    pole_elevation_multiplier_ {20.0f}
    {}
    
    FastNoise::NoiseType noise_type_;
    float noise_frequency_;
    float fractal_lacunarity_;
    float fractal_gain_;
    int fractal_octaves_;
    float extreme_multiplier_;
    float sea_level_multiplier_;
    float pole_elevation_multiplier_;
};

/**
 Class that holds and manages all tile informations for a world map.
 */
class WorldMap : public Map {
    friend class WorldBuilder;

public:
    WorldMap() {}
    WorldMap(std::shared_ptr<WorldMap> other);
    ~WorldMap() {}
    
    /**
     Gets the configuration MapConfigs for this map.
     @return A pointer to the configuration MapConfigs for this map.
     */
    std::shared_ptr<MapConfigs> GetConfigs() override { return std::static_pointer_cast<MapConfigs> (configs_); }

    /**
     Gets the map.
     @return A vector containing all the Tile in this map
     */
    std::vector<Tile*> &GetMap() override { return map_; }
        
protected:
    std::vector<Tile*> map_;
    std::shared_ptr<WorldMapConfigs> configs_;

};

}

#endif /* LIBPMG_WORLD_MAP_HPP_ */
