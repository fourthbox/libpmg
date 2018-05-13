/**
 @file world_builder
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_WORLD_BUILDER_HPP_
#define LIBPMG_WORLD_BUILDER_HPP_

#include "map_builder.hpp"
#include "world_map.hpp"
#include "world_tile.hpp"

namespace libpmg {

/**
 MapBuilder implementation for generating world maps.
 */
class WorldBuilder : MapBuilder {
public:
    WorldBuilder();
    
    /**
     Initializes every tile in the map. It must me called before applying the height map.
     */
    void InitMap() override;
    
    /**
     Reset the map and re-initializes it.
     */
    void ResetMap(bool keep_configs) override;

    /**
     Sets the extreme multiplier.
     The higher the multiplier, the more extremly high/low the heights of the height map will be generated.
     @param extreme The extreme multiplier
     */
    void SetExtremeMultiplier(float extreme);
    
    /**
     Sets the fractal gain.
     See <a href="https://github.com/Auburns/FastNoise/wiki/Noise-Settings">FastNoise settings</a> description
     @param gain The gain value
     */
    void SetFractalGain(float gain);
    
    /**
     Sets the fractas lacunarity.
     See <a href="https://github.com/Auburns/FastNoise/wiki/Noise-Settings">FastNoise settings</a> description
     @param lacunarity The lacunarity value
     */
    void SetFractalLacunarity(float lacunarity);
    
    /**
     Sets the octaves.
     See <a href="https://github.com/Auburns/FastNoise/wiki/Noise-Settings">FastNoise settings</a> description
     @param octaves The octaves value
     */
    void SetFractalOctaves(int octaves);
    
    /**
     Set the size of the map.
     @param width Map width
     @param height Map height
     */
    void SetMapSize(std::size_t width, std::size_t height) override;
    
    /**
     Sets the noise type.
     See <a href="https://github.com/Auburns/FastNoise/wiki/Noise-Settings">FastNoise settings</a> description
     @param type The noise type
     */
    void SetNoiseType(FastNoise::NoiseType type);
    
    /**
     Sets the noise frequency value.
     See <a href="https://github.com/Auburns/FastNoise/wiki/Noise-Settings">FastNoise settings</a> description
     @param frequency The noise frequency value
     */
    void SetNoiseFrequency(float frequency);
    
    /**
     Sets the pole elevation multiplier.
     The higher the value, the higher the north and south pole will be generated.
     @param pole_elevation The pole elevation multiplier value
     */
    void SetPoleElevationMultiplier(float pole_elevation);
    
    /**
     Sets the sea level multiplier.
     The higher the value, the higher the sea level will start.
     @param sea_level the sea level multiplier
     */
    void SetSeaLevelMultiplier(float sea_level);
    
    /**
     Generate the height map, using the parameters specified in the WorldConfigs.
     */
    void GenerateHeightMap();
    
    /**
     Apply the height map on the tiles.
     */
    void ApplyHeightMap();
    
    /**
     Build the map and returns a pointer.
     @return A pointer to the built map.
     */
    std::unique_ptr<Map> &Build() override;
    
private:
    std::unique_ptr<Map> map_;
    std::unique_ptr<std::unique_ptr<float[]>[]> height_map_;

};

}

#endif /* LIBPMG_WORLD_BUILDER_HPP_ */
