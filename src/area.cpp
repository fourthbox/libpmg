#include "area.hpp"

#include "rnd_manager.hpp"

namespace libpmg {
    
std::pair<size_t, size_t> Area::GetRndCoords() {
    return std::make_pair (
                           RndManager::GetInstance().GetRandomUintFromRange(
                                                                            (int)rect_.GetX(),
                                                                            (int)rect_.GetX() + (int)rect_.GetWidth()-1),
                           RndManager::GetInstance().GetRandomUintFromRange(
                                                                            (int)rect_.GetY(),
                                                                            (int)rect_.GetY() + (int)rect_.GetHeight()-1));
}
    
}
