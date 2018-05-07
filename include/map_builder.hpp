/**
 @file map_builder.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_MAP_BUILDER_HPP_
#define LIBPMG_MAP_BUILDER_HPP_

#include <memory>

#include "map.hpp"

namespace libpmg {

/**
 A pure virtual class that defines the base methods to be implemented by builder classes.
 */
class MapBuilder {
public:
    
    /**
     Initializes every tile in the map.
     */
    virtual void InitMap() = 0;
    
    /**
     Reset the map and re-initializes it.
     */
    virtual void ResetMap(bool keep_configs) = 0;
    
    /**
     Set the size of the map.
     @param width Map width
     @param height Map height
     */
    virtual void SetMapSize(std::size_t width, std::size_t height) = 0;
    
    /**
     Build the map and returns a pointer.
     @return A pointer to the built map.
     */
    virtual std::shared_ptr<Map> Build() = 0;
    
};

}
#endif /* LIBPMG_MAP_BUILDER_HPP_ */
