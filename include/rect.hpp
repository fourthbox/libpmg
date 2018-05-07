/**
 @file rect.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_RECT_HPP_
#define LIBPMG_RECT_HPP_

#include <cstddef>

namespace libpmg {

/**
 A class that holds the information of a rectangular area on a Grid.
 */
class Rect {
public:
    Rect();
    Rect(std::size_t x, std::size_t y, std::size_t width, std::size_t height);
    
    /**
     This operator enlarges the Rect by 1 tile in every direction/
     */
    Rect& operator++();
    
    /**
     This operator enlarges the Rect by 1 tile in every direction/
     */
    Rect operator++(int);
    
    /**
     This operator shrinks the Rect by 1 tile in every direction/
     */
    Rect& operator--();
    
    /**
     This operator shrinks the Rect by 1 tile in every direction/
     */
    Rect operator--(int);
    
    /**
     Gets a randomly generated Rect
     @param min_x The min X value
     @param max_x The max X value
     @param min_y The min Y value
     @param max_y The max Y value
     @param min_width The min width value
     @param max_width The max width value
     @param min_height The min height value
     @param max_height The max height value
     @return A randomly generated Rect
     */
    static Rect GetRndRect(std::size_t min_x,
                           std::size_t max_x,
                           std::size_t min_y,
                           std::size_t max_y,
                           std::size_t min_width,
                           std::size_t max_width,
                           std::size_t min_height,
                           std::size_t max_height);
    
    /**
     Gets the X coordinate.
     @return The X coordinate
     */
    inline std::size_t GetX() const          { return x_; }
    
    /**
     Gets the Y coordinate.
     @return The Y coordinate
     */
    inline std::size_t GetY() const          { return y_; }
    
    /**
     Gets the width coordinate.
     @return The width coordinate
     */
    inline std::size_t GetWidth() const      { return width_; }
    
    /**
     Gets the height coordinate.
     @return The height coordinate
     */
    inline std::size_t GetHeight() const     { return height_; }
    
    /**
     Checks whether the specified coordiantes are inside of the Rect.
     @param x The X coordinate
     @param y The Y coordinate
     @return True if the coordinates are inside the rect, false otherwise.
     */
    inline bool BoundsCheck(std::size_t x, std::size_t y) { return x < width_ + x_ && y < height_ + y_; }
    
    /**
     Checks whether the tile is directly adjacent to the room, yet outside of it
     @param x The X coordinate.
     @param y The Y coordinate.
     @return True if the tile is adjacent to the room, false otherwise
     */
    bool IsAdjacentTile(std::size_t x, std::size_t y);
    
    /**
     Prints the debug informations for this Rect
     */
    void Print();
    
private:
    std::size_t x_, y_, width_, height_;
};
    
}

#endif /* LIBPMG_RECT_HPP_ */
