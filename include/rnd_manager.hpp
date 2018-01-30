#ifndef LIBPMG_GAME_SETTINGS_HPP_
#define LIBPMG_GAME_SETTINGS_HPP_ 

#include <random>

namespace libpmg {
    
class RndManager {
public:
    static RndManager& GetInstance() {
        static RndManager instance;
        return instance;
    }
    static int seed;
    
    std::size_t GetRandomUintFromRange(std::size_t min, std::size_t max);
    void ResetInstance();
    
private:
    RndManager();
    
    RndManager(RndManager const&) = delete;
    void operator=(RndManager const&) = delete;
    
    std::mt19937 *random_generator_;
};
    
}
#endif /* LIBPMG_GAME_SETTINGS_HPP_ */
