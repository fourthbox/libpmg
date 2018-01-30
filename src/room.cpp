#include "room.hpp"

#include <sstream>

#include "utils.hpp"

namespace libpmg {
    
void Room::Print() {
    
    std::ostringstream stream;
    stream
    << "Room: X:" << rect_.GetX()
    << ", Y:" << rect_.GetY()
    << ", width:" << rect_.GetWidth()
    << ", height:" << rect_.GetHeight();
    
    Utils::LogDebug(stream.str());
}
        
}
