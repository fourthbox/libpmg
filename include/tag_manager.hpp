#ifndef LIBPMG_TAG_MANAGER_HPP_
#define LIBPMG_TAG_MANAGER_HPP_

#include <unordered_map>
#include <vector>

#include "tag.hpp"
#include "taggable.hpp"

namespace libpmg {

class TagManager {

    public:
        static TagManager& GetInstance() {
            static TagManager instance;
            return instance;
        }

        TagManager(TagManager const&) = delete;
        void operator=(TagManager const&) = delete;

        bool TryAddTaggable(Taggable &taggable, std::shared_ptr<Tag> tag);
        void RemoveTaggable(Taggable &taggable, std::shared_ptr<Tag> tag);
    
    std::shared_ptr<Tag> floor_tag, wall_tag, door_tag, explored_tag;

    private:
        TagManager();

        std::unordered_map<std::shared_ptr<Tag>, std::vector<Taggable*>> tag_map_;
};

}

#endif /* LIBPMG_TAG_MANAGER_HPP_ */
