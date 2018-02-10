/**
 @file tag.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_TAG_HPP_
#define LIBPMG_TAG_HPP_

#include <memory>
#include <string>

namespace libpmg {

/**
 A class representing a tag that can be applied to a Taggable class.
 */
class Tag {
public:
    char sprite_;            /**< The char "sprite" that will be used for printing this Tag. */
    float draw_priority_;    /**< When there are multiple tags on a Taggable object, only the sprite with the higher draw_priority value will be drawn. */
    std::string name_;       /**< The name of this Tag. */
    
    Tag(char sprite, float draw_priority, std::string nm);
    
    bool operator==(const Tag &other);
};
    
}

#endif /* LIBPMG_TAG_HPP_ */
