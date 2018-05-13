#include "tag_manager.hpp"

#include <algorithm>

#include "constants.hpp"
#include "tile.hpp"

namespace libpmg {

typedef std::shared_ptr<Tag> Tag_p;

TagManager::TagManager() {
    floor_tag_ = std::make_shared<Tag>(Tag(kDefaultEmptyChar, kDefaultEmptyDrawPriority, "floor"));
    wall_tag_ = std::make_shared<Tag>(Tag(kDefaultWallChar, kDefaultWallDrawPriority, "wall"));
    door_tag_ = std::make_shared<Tag>(Tag(kDefaultDoorChar, kDefaultDoorDrawPriority, "door"));
    explored_tag_ = std::make_shared<Tag>(Tag(kDefaultEmptyChar, kDefaultEmptyDrawPriority, "explored"));
    upstairs_tag_ = std::make_shared<Tag>(Tag(kDefaultUpstairChar, kDefaultStairDrawPriority, "upstarirs"));
    downstairs_tag_ = std::make_shared<Tag>(Tag(kDefaultDownstairChar, kDefaultStairDrawPriority, "downstarirs"));
}

void TagManager::RemoveTaggable(libpmg::Taggable *taggable, Tag_p tag) {
    tag_map_[tag].erase(
            std::remove(tag_map_[tag].begin(), tag_map_[tag].end(), taggable),
            tag_map_[tag].end());
}

bool TagManager::TryAddTaggable(libpmg::Taggable *taggable, Tag_p tag) {
    if(std::find(tag_map_[tag].begin(), tag_map_[tag].end(), taggable) != tag_map_[tag].end())
        return false;

    tag_map_[tag].push_back(taggable);

    return true;
}

}
