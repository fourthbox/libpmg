/**
 @file tagable.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_TAGGABLE_HPP_
#define LIBPMG_TAGGABLE_HPP_

#include <initializer_list>
#include <memory>
#include <vector>

namespace libpmg {
    
class Tag;

/**
 This class represent an object that upon which a Tag can be applied.
 It holds a list of tags. Its information should always be synced with the TagManager.
 */
class Taggable {
public:
    class TagManager;
    
    Taggable(std::shared_ptr<Tag> tag) : Taggable ({tag}) {}
    Taggable(std::initializer_list<std::shared_ptr<Tag>> tags) { AddTags(tags); }
    virtual ~Taggable() = 0;
    
    /**
     Add the specified Tag to this object.
     @param tag A pointer to a tag to be added
     */
    void AddTag(std::shared_ptr<Tag> tag);
    
    /**
     Add the specified Tag list to this object.
     @param tags A Tag list to to be added
     */
    void AddTags(std::initializer_list<std::shared_ptr<Tag>> tags);
    
    /**
     Checks whether this object has the specified Tag.
     @param tag The Tag to check
     @return True if this object has a reference to the tag, false otherwise
     */
    bool HasTag(std::shared_ptr<Tag> tag);
    
    /**
     Checks whether this object has any Tag in the specified list.
     @param tags A Tag list to check
     @return True if this object has a reference to any tag, false otherwise
     */
    bool HasAnyTag(std::initializer_list<std::shared_ptr<Tag>> tags);
    
    /**
     Removes the specified Tag to this object.
     @param tag A pointer to a tag to be removed
     */
    void RemoveTag(std::shared_ptr<Tag> tag);
    
    /**
     Removes the specified Tag list to this object.
     @param tags A Tag list to to be removed
     */
    void RemoveTags(std::initializer_list<std::shared_ptr<Tag>> tags);
    
    /**
     Insert and removes the specified tags from the object.
     @param to_insert A tag list to be added
     @param to_remove A tag list to be removed
     */
    void UpdateTags(std::initializer_list<std::shared_ptr<Tag>> to_insert, std::initializer_list<std::shared_ptr<Tag>> to_remove = {});
    
protected:
    std::vector<std::shared_ptr<Tag>> tags_;    /**< The tags assigned to this object. */
};
    
}

#endif /* LIBPMG_TAGGABLE_HPP_ */
