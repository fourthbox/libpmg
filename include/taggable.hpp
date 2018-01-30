#ifndef LIBPMG_TAGGABLE_HPP_
#define LIBPMG_TAGGABLE_HPP_

#include <initializer_list>
#include <memory>
#include <vector>

namespace libpmg {
    
class Tag;

class Taggable {
    
public:
    class TagManager;
    
    Taggable(std::shared_ptr<Tag> tag) : Taggable ({tag}) {}
    Taggable(std::initializer_list<std::shared_ptr<Tag>> tags) { AddTags(tags); }
    virtual ~Taggable() = 0;
    
    void AddTag(std::shared_ptr<Tag> tag);
    void AddTags(std::initializer_list<std::shared_ptr<Tag>> tags);
    bool HasTag(std::shared_ptr<Tag> tag);
    bool HasAnyTag(std::initializer_list<std::shared_ptr<Tag>> tags);
    void RemoveTag(std::shared_ptr<Tag> tag);
    void RemoveTags(std::initializer_list<std::shared_ptr<Tag>> tags);
    void UpdateTags(std::initializer_list<std::shared_ptr<Tag>> to_insert, std::initializer_list<std::shared_ptr<Tag>> to_remove = {});
    
protected:
    std::vector<std::shared_ptr<Tag>> tags_;
};
    
}

#endif /* LIBPMG_TAGGABLE_HPP_ */
