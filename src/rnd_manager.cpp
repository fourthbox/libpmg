#include "rnd_manager.hpp"

#include "constants.hpp"

namespace libpmg {
    
int RndManager::seed_ = kDefaultSeed;

size_t RndManager::GetRandomUintFromRange(size_t min, size_t max) {
    std::uniform_int_distribution random_distributor((int)min, (int)max);
    return random_distributor(*random_generator_);
}

RndManager::RndManager() {
    if (random_generator_ == nullptr)
        ResetInstance();
}

void RndManager::ResetInstance() {
    random_generator_ = std::make_shared<std::mt19937> (seed_);
}
    
}
