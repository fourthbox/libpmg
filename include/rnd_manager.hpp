/**
 @file rnd_manager.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_GAME_SETTINGS_HPP_
#define LIBPMG_GAME_SETTINGS_HPP_ 

#include <random>

namespace libpmg {

/**
 This singletone class manages the random generator.
 */
class RndManager {
public:
    
    /**
     Use this to access the class.
     @return A singleton reference to this class
     */
    static RndManager& GetInstance() {
        static RndManager instance;
        return instance;
    }
    static int seed_;    /**< The seed used for generation. Instance must be reset after changing the seed */
    
    /**
     Gets a random unsigned int from a range.
     @param min The min value (inclusive)
     @param max Tha max value (inclusive)
     @return A random unsigned integer bigger or equal than min and smaller or equal to max
     */
    std::size_t GetRandomUintFromRange(std::size_t min, std::size_t max);
    
    /**
     Reset the instance and applies the seed.
     */
    void ResetInstance();
    
private:
    RndManager();
    
    RndManager(RndManager const&) = delete;
    void operator=(RndManager const&) = delete;
    
    std::mt19937 *random_generator_;
};
    
}
#endif /* LIBPMG_GAME_SETTINGS_HPP_ */
