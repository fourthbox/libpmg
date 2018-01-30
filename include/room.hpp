#ifndef LIBPMG_ROOM_HPP_
#define LIBPMG_ROOM_HPP_

#include "area.hpp"

namespace libpmg {
    
class Room : public Area {
public:
    Room(Rect rect) : Area(rect) {}
    
    void Print();
    
};
    
}

#endif /* LIBPMG_ROOM_HPP_ */
