/**
 @file location.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_LOCATION_HPP_
#define LIBPMG_LOCATION_HPP_

#include <memory>
#include <utility>

namespace libpmg {

/**
 A class holding the information for a location on a Grid.
 */
class Location {
public:
    Location(std::size_t x, std::size_t y);
    Location(std::pair<std::size_t, std::size_t> xy);
    
    bool is_explored_;       /**< Whether the tile has been explored or not. Only used for by the path finder. */
    float cost_;             /**< The cost used by the path finding algorithm */
    
    inline std::size_t GetX() { return coords_.first; }
    inline std::size_t GetY() { return coords_.second; }
    inline std::pair<std::size_t, std::size_t> GetXY() { return coords_; }
    
protected:
    std::pair<std::size_t, std::size_t> coords_;    /**< The coordinates held by this class */
    
};
    
}

#endif /* LIBPMG_LOCATION_HPP_ */
