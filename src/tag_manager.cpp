#include "tag_manager.hpp"

#include <algorithm>

#include "map_configs.hpp"
#include "tile.hpp"

namespace libpmg {

typedef std::shared_ptr<Tag> Tag_p;

TagManager::TagManager() {
    floor_tag = std::make_shared<Tag>(Tag(kDefaultEmptyChar, kDefaultEmptyDrawPriority, "floor"));
    wall_tag = std::make_shared<Tag>(Tag(kDefaultWallChar, kDefaultWallDrawPriority, "wall"));
    door_tag = std::make_shared<Tag>(Tag(kDefaultDoorChar, kDefaultDoorDrawPriority, "door"));
    explored_tag = std::make_shared<Tag>(Tag(kDefaultEmptyChar, kDefaultEmptyDrawPriority, "explored"));
}

void TagManager::RemoveTaggable(libpmg::Taggable &taggable, Tag_p tag) {
    tag_map_[tag].erase(
            std::remove(tag_map_[tag].begin(), tag_map_[tag].end(), &taggable),
            tag_map_[tag].end());
}

bool TagManager::TryAddTaggable(libpmg::Taggable &taggable, Tag_p tag) {
    if(std::find(tag_map_[tag].begin(), tag_map_[tag].end(), &taggable) != tag_map_[tag].end())
        return false;

    tag_map_[tag].push_back(&taggable);

    return true;
}

}
