#ifndef LIBPMG_LOCATION_HPP_
#define LIBPMG_LOCATION_HPP_

#include <memory>
#include <utility>

namespace libpmg {
    
class Location {
public:
    Location(std::size_t x, std::size_t y);
    Location(std::pair<std::size_t, std::size_t> xy);
    
    bool is_explored;
    float cost;
    
    inline std::size_t GetX() { return coords_.first; }
    inline std::size_t GetY() { return coords_.second; }
    inline std::pair<std::size_t, std::size_t> GetXY() { return coords_; }
    
protected:
    std::pair<std::size_t, std::size_t> coords_;
    
};
    
}

#endif /* LIBPMG_LOCATION_HPP_ */
