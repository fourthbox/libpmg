/**
 @file area.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_AREA_HPP_
#define LIBPMG_AREA_HPP_

#include <utility>

#include "rect.hpp"

namespace libpmg {

/**
 A pure virtual class representing an area.
 */
class Area {
    
protected:
    Rect rect_;
    
public:
    Area(Rect rect) :
    rect_(rect)
    {}
    
    inline Rect &GetRect() { return rect_; }
    
    /**
     Get a pair of random coordinates from within the defined area.
     @return A pair containing 2 random values.
     */
     std::pair<std::size_t, std::size_t> GetRndCoords() const;
    
    virtual void Print() = 0;
};
    
}

#endif /* LIBPMG_AREA_HPP_ */
