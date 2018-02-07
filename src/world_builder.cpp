#include "world_builder.hpp"

#include <cassert>

#include "rnd_manager.hpp"
#include "utils.hpp"

namespace libpmg {
    
WorldBuilder::WorldBuilder() {
    map_ = std::make_shared<WorldMap>();
}

std::shared_ptr<Map> WorldBuilder::Build() {
    if (map_->map_.empty()) {
        Utils::LogError("WorldBuilder::Build", "Map has not been not initialized.\nAborting...");
        abort();
    }
    
    return map_;
}

void WorldBuilder::InitMap() {
//        for (auto i {0}; i < map_->configs_->map_height_; i++) {
//            for (auto j {0}; j < map_->configs_->map_width_; j++)
//                map_->map_.push_back(std::make_shared<Tile>(Tile (j, i, {TagManager::GetInstance().wall_tag})));
//        }
}

void WorldBuilder::GenerateHeightMap() {
    FastNoise noise_map;
    noise_map.SetSeed(RndManager::seed);
    noise_map.SetNoiseType(map_->world_configs->noise_type_);
    noise_map.SetFrequency(map_->world_configs->noise_frequency_);
    noise_map.SetFractalLacunarity(map_->world_configs->fractal_lacunarity_);
    noise_map.SetFractalGain(map_->world_configs->fractal_gain_);
    noise_map.SetFractalOctaves(map_->world_configs->fractal_octaves_);
    
    height_map_ = std::make_unique< std::unique_ptr<float[]>[]>(map_->configs_->map_height_);
    
    for (auto i {0}; i < map_->configs_->map_height_; i++) {
        
        auto temp_height_map {std::make_unique<float[]>(map_->configs_->map_width_)};
        for (auto j {0}; j < map_->configs_->map_width_; j++) {
            auto noise {noise_map.GetNoise(j, i)};
            
            // Multiplier for more extreme heights
            noise *= map_->world_configs->extreme_multiplier_;
            
            // Transform from -1..1 to 0..1
            noise += 1.0f;
            noise /= 2.0f;
            
            // Raise sea levels
            noise = pow(noise, map_->world_configs->sea_level_multiplier_);
            
            // Error check in case of NaN
            if (noise != noise)
                noise = 0.0f;
            
            // Add high lands on both poles
            if (i >= map_->configs_->map_height_/6) {
                temp_height_map[j] = noise +
                    pow(
                        ((float)i) / ((float)map_->configs_->map_height_),
                        map_->world_configs->pole_elevation_multiplier_);
            } else if (i <= map_->configs_->map_height_/6) {
                temp_height_map[j] = noise +
                    pow(
                        ((float)map_->configs_->map_height_ - (float)i) / (float)map_->configs_->map_height_,
                        map_->world_configs->pole_elevation_multiplier_);
            }
        }
        
        height_map_[i] = std::move(temp_height_map);
    }

}

void WorldBuilder::ResetMap() {
    this->InitMap();
}

void WorldBuilder::SetExtremeMultiplier(float extreme) {
    assert (!map_->map_.empty());
    
    map_->world_configs->extreme_multiplier_ = extreme;
}

void WorldBuilder::SetFractalGain(float gain) {
    assert (!map_->map_.empty());

    map_->world_configs->fractal_gain_ = gain;
}

void WorldBuilder::SetFractalLacunarity(float lacunarity) {
    assert (!map_->map_.empty());

    map_->world_configs->fractal_lacunarity_ = lacunarity;
}

void WorldBuilder::SetFractalOctaves(int octaves) {
    assert (!map_->map_.empty());

    map_->world_configs->fractal_octaves_ = octaves;
}

void WorldBuilder::SetMapSize(std::size_t width, std::size_t height) {
    assert (!map_->map_.empty());

    map_->configs_->map_width_ = width;
    map_->configs_->map_height_ = height;
    
}

void WorldBuilder::SetNoiseType(FastNoise::NoiseType type) {
    assert (!map_->map_.empty());

    map_->world_configs->noise_type_ = type;
}

void WorldBuilder::SetNoiseFrequency(float frequency) {
    assert (!map_->map_.empty());

    map_->world_configs->noise_frequency_ = frequency;
}

void WorldBuilder::SetPoleElevationMultiplier(float pole_elevation) {
    assert (!map_->map_.empty());

    map_->world_configs->pole_elevation_multiplier_ = pole_elevation;
}

void WorldBuilder::SetSeaLevelMultiplier(float sea_level) {
    assert (!map_->map_.empty());

    map_->world_configs->sea_level_multiplier_ = sea_level;
}
    
}
