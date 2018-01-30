#ifndef LIBPMG_AREA_HPP_
#define LIBPMG_AREA_HPP_

#include <utility>

#include "rect.hpp"

namespace libpmg {

class Area {
    
protected:
    Rect rect_;
    
public:
    Area(Rect rect) :
    rect_(rect)
    {}
    
    inline Rect GetRect() { return rect_; }
    std::pair<std::size_t, std::size_t> GetRndCoords();
    
    virtual void Print() = 0;
};
    
}

#endif /* LIBPMG_AREA_HPP_ */
