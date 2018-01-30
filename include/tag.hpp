#ifndef LIBPMG_TAG_HPP_
#define LIBPMG_TAG_HPP_

#include <memory>
#include <string>

namespace libpmg {
    
class Tag {
public:
    char sprite;
    float draw_priority;
    std::string name;
    
    Tag(char sprite, float draw_priority, std::string nm);
    
    bool operator==(const Tag &other);
};
    
}

#endif /* LIBPMG_TAG_HPP_ */
