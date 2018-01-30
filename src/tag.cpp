#include "tag.hpp"

namespace libpmg {
    
Tag::Tag(char sprite, float draw_priority, std::string nm)
: sprite {sprite},
draw_priority {draw_priority},
name {nm}
{}

bool Tag::operator==(const Tag &other) {
    return name == other.name;
}
    
}
