#ifndef LIBPMG_TILE_HPP_
#define LIBPMG_TILE_HPP_

#include "location.hpp"
#include "tag_manager.hpp"

namespace libpmg {
    
class Tile : public Location, public Taggable {
public:
    Tile (std::size_t x, std::size_t y, std::initializer_list<std::shared_ptr<Tag>> tags = {libpmg::TagManager::GetInstance().wall_tag});
    ~Tile() override = default;
    
    char GetChar();
};
    
}

#endif /* LIBPMG_TILE_HPP_ */
