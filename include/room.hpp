/**
 @file room.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_ROOM_HPP_
#define LIBPMG_ROOM_HPP_

#include "area.hpp"

namespace libpmg {

/**
 Class representing a room inside of a Map
 */
class Room : public Area {
public:
    Room(Rect rect) : Area(rect) {}

    /**
     Prints debug information about this Room.
     */
    void Print();
};
    
}

#endif /* LIBPMG_ROOM_HPP_ */
