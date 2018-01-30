#include "rect.hpp"

#include <cassert>
#include <sstream>

#include "rnd_manager.hpp"
#include "utils.hpp"

namespace libpmg {
    
Rect::Rect()
: x_ {0},
y_ {0},
width_ {0},
height_ {0}
{}

Rect::Rect(size_t x, size_t y, size_t width, size_t height)
: x_ {x},
y_ {y},
width_ {width},
height_ {height}
{}

Rect& Rect::operator++() {
    this->x_ -= 1;
    this->y_ -= 1;
    this->width_ += 2;
    this->height_ += 2;
    
    return *this;
}

Rect Rect::operator++(int) {
    auto result {*this};
    ++(*this);
    return result;
}

Rect& Rect::operator--() {
    assert(width_ > 2 && height_ > 2);
    this->x_ += 1;
    this->y_ += 1;
    this->width_ -= 2;
    this->height_ -= 2;
    
    return *this;
}

Rect Rect::operator--(int) {
    auto result {*this};
    --(*this);
    return result;
}

Rect Rect::GetRndRect(MapConfigs configs) {
    return GetRndRect(0, configs.map_width_-1,
                      0, configs.map_height_-1,
                      configs.min_room_width_, configs.max_room_width_,
                      configs.min_room_height_, configs.max_room_height_);
}

Rect Rect::GetRndRect(size_t min_x,
                      size_t max_x,
                      size_t min_y,
                      size_t max_y,
                      size_t min_width,
                      size_t max_width,
                      size_t min_height,
                      size_t max_height) {
    
    return Rect(RndManager::GetInstance().GetRandomUintFromRange(min_x, max_x),
                RndManager::GetInstance().GetRandomUintFromRange(min_y, max_y),
                RndManager::GetInstance().GetRandomUintFromRange(min_width, max_width),
                RndManager::GetInstance().GetRandomUintFromRange(min_height, max_height));
}

void Rect::Print() {
    std::ostringstream stream;
    stream
    << "Rect: X:" << x_
    << ", Y:" << y_
    << ", width:" << width_
    << ", height:" << height_;
    
    Utils::LogDebug(stream.str());
}
    
bool Rect::IsAdjacentTile(std::size_t x, std::size_t y) {
    if (BoundsCheck(x, y))
        return false;
    
    return (*this)++.BoundsCheck(x, y);
}

}
