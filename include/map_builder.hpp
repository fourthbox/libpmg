#ifndef LIBPMG_MAP_BUILDER_HPP_
#define LIBPMG_MAP_BUILDER_HPP_

#include <memory>

#include "map.hpp"

namespace libpmg {

class MapBuilder {
public:    
    virtual void InitMap() = 0;
    virtual void ResetMap() = 0;
    virtual void SetMapSize(std::size_t width, std::size_t height) = 0;
    
    virtual std::shared_ptr<Map> Build() = 0;
    
};

}
#endif /* LIBPMG_MAP_BUILDER_HPP_ */
