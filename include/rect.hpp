#ifndef LIBPMG_RECT_HPP_
#define LIBPMG_RECT_HPP_

#include <cstddef>

namespace libpmg {
    
class Rect {
public:
    Rect();
    Rect(std::size_t x, std::size_t y, std::size_t width, std::size_t height);
    
    Rect& operator++();
    Rect operator++(int);
    Rect& operator--();
    Rect operator--(int);
    
    static Rect GetRndRect(std::size_t min_x,
                           std::size_t max_x,
                           std::size_t min_y,
                           std::size_t max_y,
                           std::size_t min_width,
                           std::size_t max_width,
                           std::size_t min_height,
                           std::size_t max_height);
    
    inline std::size_t GetX()          { return x_; }
    inline std::size_t GetY()          { return y_; }
    inline std::size_t GetWidth()      { return width_; }
    inline std::size_t GetHeight()     { return height_; }
    
    inline bool BoundsCheck(std::size_t x, std::size_t y) { return x < width_ + x_ && y < height_ + y_; }
    
    /**
     Check whether the tile is directly adjacent to the room, yet outside of it
     
     @param x X coordinate.
     @param y Y coordinate.
     @return True if the tile is adjacent to the room, false otherwise
     bar and "," as the half bar.
     */
    bool IsAdjacentTile(std::size_t x, std::size_t y);
    
    void Print();
    
private:
    std::size_t x_, y_, width_, height_;
};
    
}

#endif /* LIBPMG_RECT_HPP_ */
