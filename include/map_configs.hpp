#ifndef LIBPMG_MAP_CONFIGS_HPP_
#define LIBPMG_MAP_CONFIGS_HPP_

namespace libpmg {

static const int kDefaultSeed                       {666};
static const float kDefaultEmptyTileCost            {0.0f};
static const float kDefaultWallTileCost             {666.0f};

static const std::size_t kDefaultMapWidth                {185};
static const std::size_t kDefaultMapHeight               {45};
static const std::size_t kDefaultRoomQuantity            {20};
static const std::size_t kDefaultRoomPlacementAttempts   {10};
static const std::size_t kDefaultMaxRoomWidth            {12};
static const std::size_t kDefaultMaxRoomHeight           {12};
static const std::size_t kDefaultMinRoomWidth            {4};
static const std::size_t kDefaultMinRoomHeight           {4};

static const char kDefaultEmptyChar                 {'.'};
static const char kDefaultWallChar                  {'#'};
static const char kDefaultDoorChar                  {'+'};
static const float kDefaultEmptyDrawPriority        {50.0f};
static const float kDefaultWallDrawPriority         {60.0f};
static const float kDefaultDoorDrawPriority         {70.0f};

struct MapConfigs {
    MapConfigs()
    : map_width_ {kDefaultMapWidth},
    map_height_ {kDefaultMapHeight},
    rooms_ {kDefaultRoomQuantity},
    max_room_placement_attempts_ {kDefaultRoomPlacementAttempts},
    max_room_width_ {kDefaultMaxRoomWidth},
    max_room_height_ {kDefaultMaxRoomHeight},
    min_room_width_ {kDefaultMinRoomWidth},
    min_room_height_ {kDefaultMinRoomHeight}
    {}
    
    std::size_t map_width_;
    std::size_t map_height_;
    std::size_t rooms_;
    std::size_t max_room_placement_attempts_;
    std::size_t max_room_width_;
    std::size_t max_room_height_;
    std::size_t min_room_width_;
    std::size_t min_room_height_;
};

}

#endif /* LIBPMG_MAP_CONFIGS_HPP_ */
