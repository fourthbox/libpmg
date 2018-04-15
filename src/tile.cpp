#include "tile.hpp"

namespace libpmg {
    
Tile::Tile (size_t x, size_t y, std::initializer_list<std::shared_ptr<Tag>> tags)
: Location (x, y),
Taggable (tags)
{}
    
Tile::Tile (size_t x, size_t y, std::vector<std::shared_ptr<Tag>> tags)
: Location (x, y),
Taggable (tags)
{}

char Tile::GetChar() {
    auto priority {0.0f};
    auto sprite {'.'};
    
    for (auto const &tag : tags_) {
        if (tag->draw_priority_ > priority) {
            sprite = tag->sprite_;
            priority = tag->draw_priority_;
        }
    }
    
    return sprite;
}
    
}
