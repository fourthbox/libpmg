/**
 The header containing all the constants needed.
 It's mostly default values.
 @file constants.hpp
 @author pat <pat@fourthbox.com>
 */


#ifndef LIBPMG_CONSTANTS_HPP_
#define LIBPMG_CONSTANTS_HPP_

namespace libpmg {

static const int kDefaultSeed                       {666};
static const float kDefaultEmptyTileCost            {0.0f};
static const float kDefaultWallTileCost             {666.0f};

static const char kDefaultEmptyChar                 {'.'};
static const char kDefaultWallChar                  {'#'};
static const char kDefaultDoorChar                  {'+'};
static const float kDefaultEmptyDrawPriority        {0.5f};
static const float kDefaultWallDrawPriority         {0.6f};
static const float kDefaultDoorDrawPriority         {0.7f};

}

#endif /* LIBPMG_CONSTANTS_HPP_ */
