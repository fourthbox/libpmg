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
    Room(Rect rect, size_t x_position, size_t y_position);
    
    /**
     Prints debug information about this Room.
     */
    void Print();
    
private:
    size_t x_position, y_position;  /**< The top left coordinates of the room in the map.*/
};
    
}

#endif /* LIBPMG_ROOM_HPP_ */
