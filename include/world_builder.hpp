#ifndef LIBPMG_WORLD_BUILDER_HPP_
#define LIBPMG_WORLD_BUILDER_HPP_

#include "map_builder.hpp"
#include "world_map.hpp"

namespace libpmg {

    class WorldBuilder : MapBuilder {
    public:
        WorldBuilder();
        
        void InitMap() override;
        void ResetMap() override;
        
        void SetExtremeMultiplier(float extreme);
        void SetFractalGain(float gain);
        void SetFractalLacunarity(float lacunarity);
        void SetFractalOctaves(int octaves);
        void SetMapSize(std::size_t width, std::size_t height) override;
        void SetNoiseType(FastNoise::NoiseType type);
        void SetNoiseFrequency(float frequency);
        void SetPoleElevationMultiplier(float pole_elevation);
        void SetSeaLevelMultiplier(float sea_level);
        
        void GenerateHeightMap();
        void GenerateTiles();
        
        std::shared_ptr<Map> Build() override;
        
    private:
        std::shared_ptr<WorldMap> map_;
        std::unique_ptr<std::unique_ptr<float[]>[]> height_map_;
        
    };

}

#endif /* LIBPMG_WORLD_BUILDER_HPP_ */
