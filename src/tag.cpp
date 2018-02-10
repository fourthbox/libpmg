#include "tag.hpp"

namespace libpmg {
    
Tag::Tag(char sprite, float draw_priority, std::string nm)
: sprite_ {sprite},
draw_priority_ {draw_priority},
name_ {nm}
{}

bool Tag::operator==(const Tag &other) {
    return name_ == other.name_;
}
    
}
