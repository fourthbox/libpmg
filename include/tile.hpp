/**
 @file tile.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_TILE_HPP_
#define LIBPMG_TILE_HPP_

#include "location.hpp"
#include "tag_manager.hpp"

namespace libpmg {

/**
 A class representing a taggable location on a Map
 */
class Tile : public Location, public Taggable {
public:
    Tile (std::size_t x, std::size_t y, std::initializer_list<std::shared_ptr<Tag>> tags = {libpmg::TagManager::GetInstance().wall_tag_});
    ~Tile() override = default;
    
    /**
     Gets the char used to print this tile.
     The char is selected from the Tag with the highest draw priority. If no tags are preent, a default char will be used.
     @return The char to be used to print this tile
     */
    char GetChar();
};
    
}

#endif /* LIBPMG_TILE_HPP_ */
