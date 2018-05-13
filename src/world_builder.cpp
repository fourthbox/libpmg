#include "world_builder.hpp"

#include <cassert>

#include "rnd_manager.hpp"
#include "utils.hpp"

namespace libpmg {
    
WorldBuilder::WorldBuilder() {
    map_ = std::make_unique<WorldMap>();
    height_map_ = nullptr;
}

std::unique_ptr<Map> &WorldBuilder::Build() {
    if (map_->GetMap()->empty()) {
        Utils::LogError("WorldBuilder::Build", "Map has not been not initialized.\nAborting...");
        abort();
    }
    
    return map_;
}

void WorldBuilder::InitMap() {
    for (auto i {0}; i < map_->GetConfigs().map_height_; i++) {
        for (auto j {0}; j < map_->GetConfigs().map_width_; j++)
            map_->GetMap()->push_back (std::make_unique<WorldTile> (j, i));
    }
}

void WorldBuilder::GenerateHeightMap() {
    auto world_configs {(WorldMapConfigs&)map_->GetConfigs()};
    
    FastNoise noise_map;
    noise_map.SetSeed(RndManager::seed_);
    noise_map.SetNoiseType(world_configs.noise_type_);
    noise_map.SetFrequency(world_configs.noise_frequency_);
    noise_map.SetFractalLacunarity(world_configs.fractal_lacunarity_);
    noise_map.SetFractalGain(world_configs.fractal_gain_);
    noise_map.SetFractalOctaves(world_configs.fractal_octaves_);
    
    height_map_ = std::make_unique< std::unique_ptr<float[]>[]>(world_configs.map_height_);
    
    for (auto i {0}; i < world_configs.map_height_; i++) {
        
        auto temp_height_map {std::make_unique<float[]>(world_configs.map_width_)};
        for (auto j {0}; j < world_configs.map_width_; j++) {
            auto noise {noise_map.GetNoise(j, i)};
            
            // Multiplier for more extreme heights
            noise *= world_configs.extreme_multiplier_;
            
            // Transform from -1..1 to 0..1
            noise += 1.0f;
            noise /= 2.0f;
            
            // Raise sea levels
            noise = pow(noise, world_configs.sea_level_multiplier_);
            
            // Error check in case of NaN
            if (noise != noise)
                noise = 0.0f;
            
            // Add high lands on both poles
            if (i >= world_configs.map_height_/6) {
                temp_height_map[j] = noise +
                    pow(
                        ((float)i) / ((float)world_configs.map_height_),
                        world_configs.pole_elevation_multiplier_);
            } else if (i <= world_configs.map_height_/6) {
                temp_height_map[j] = noise +
                    pow(
                        ((float)world_configs.map_height_ - (float)i) / (float)world_configs.map_height_,
                        world_configs.pole_elevation_multiplier_);
            }
        }
        
        height_map_[i] = std::move(temp_height_map);
    }
}

void WorldBuilder::ApplyHeightMap() {
    assert (height_map_ != nullptr);

    for (auto i {0}; i < map_->GetConfigs().map_height_; i++) {
        for (auto j {0}; j < map_->GetConfigs().map_width_; j++) {
            auto tile = (WorldTile*)map_->GetTile(j, i);
            tile->altitude_ = height_map_[i][j];
        }
    }
}

void WorldBuilder::ResetMap(bool keep_configs) {
    this->InitMap();
    // keep configs
    this->height_map_.reset();
}

void WorldBuilder::SetExtremeMultiplier(float extreme) {
    assert (map_->GetMap()->empty());
    
    ((WorldMapConfigs&)map_->GetConfigs()).extreme_multiplier_ = extreme;
}

void WorldBuilder::SetFractalGain(float gain) {
    assert (map_->GetMap()->empty());

   ((WorldMapConfigs&)map_->GetConfigs()).fractal_gain_ = gain;
}

void WorldBuilder::SetFractalLacunarity(float lacunarity) {
    assert (map_->GetMap()->empty());

    ((WorldMapConfigs&)map_->GetConfigs()).fractal_lacunarity_ = lacunarity;
}

void WorldBuilder::SetFractalOctaves(int octaves) {
    assert (map_->GetMap()->empty());

    ((WorldMapConfigs&)map_->GetConfigs()).fractal_octaves_ = octaves;
}

void WorldBuilder::SetMapSize(std::size_t width, std::size_t height) {
    assert (map_->GetMap()->empty());

    map_->GetConfigs().map_width_ = width;
    map_->GetConfigs().map_height_ = height;
}

void WorldBuilder::SetNoiseType(FastNoise::NoiseType type) {
    assert (map_->GetMap()->empty());

    ((WorldMapConfigs&)map_->GetConfigs()).noise_type_ = type;
}

void WorldBuilder::SetNoiseFrequency(float frequency) {
    assert (map_->GetMap()->empty());

    ((WorldMapConfigs&)map_->GetConfigs()).noise_frequency_ = frequency;
}

void WorldBuilder::SetPoleElevationMultiplier(float pole_elevation) {
    assert (map_->GetMap()->empty());

    ((WorldMapConfigs&)map_->GetConfigs()).pole_elevation_multiplier_ = pole_elevation;
}

void WorldBuilder::SetSeaLevelMultiplier(float sea_level) {
    assert (map_->GetMap()->empty());

    ((WorldMapConfigs&)map_->GetConfigs()).sea_level_multiplier_ = sea_level;
}
    
}
