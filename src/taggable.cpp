#include "taggable.hpp"

#include <algorithm>

#include "tag_manager.hpp"

namespace libpmg {
    
typedef std::shared_ptr<Tag> Tag_p;

Taggable::~Taggable()
{}

void Taggable::AddTags(std::initializer_list<Tag_p> tags) {
    for (auto const &tag : tags) AddTag(tag);
}
    
void Taggable::AddTags(std::vector<Tag_p> tags) {
    for (auto const &tag : tags) AddTag(tag);
}

void Taggable::RemoveTags(std::initializer_list<Tag_p> tags) {
    for (auto const &tag : tags) RemoveTag(tag);
}

void Taggable::UpdateTags(std::initializer_list<Tag_p> to_insert, std::initializer_list<Tag_p> to_remove) {
    for (auto const &tag : to_insert)
        this->AddTag(tag);
    
    for (auto const &tag : to_remove)
        this->RemoveTag(tag);
}

void Taggable::AddTag(Tag_p tag) {
    if (HasTag(tag))
        return;
    
    tags_.push_back(tag);
    libpmg::TagManager::GetInstance().TryAddTaggable(*this, tag);
}

bool Taggable::HasTag(Tag_p tag) {
    if (tags_.empty() || tags_.size() <= 0) {
        return false;
    }
    
    for (auto const &value : tags_) {
        if (*value == *tag)
            return true;
    }
    
    return false;
}

bool Taggable::HasAnyTag(std::initializer_list<Tag_p> tags) {
    for (auto const &tag : tags) {
        if (HasTag(tag))
            return true;
    }
    return false;
}

void Taggable::RemoveTag(Tag_p tag) {
    if (!HasTag(tag))
        return;
    
    tags_.erase(std::remove(tags_.begin(),
                            tags_.end(),
                            tag),
                tags_.end());
    libpmg::TagManager::GetInstance().RemoveTaggable(*this, tag);
}

}
