#ifndef LIBPMG_WORLD_MAP_HPP_
#define LIBPMG_WORLD_MAP_HPP_

#include "FastNoise.h"
#include "map.hpp"

namespace libpmg {
    
class WorldMapConfigs : public MapConfigs {
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
    
class WorldMap : public Map {
    friend class WorldBuilder;

public:
    WorldMap();
    WorldMap(std::shared_ptr<WorldMap> other);
    
    std::shared_ptr<WorldMapConfigs> world_configs;
};

}

#endif /* LIBPMG_WORLD_MAP_HPP_ */
