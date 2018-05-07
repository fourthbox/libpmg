/**
 @file tag_manager.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_TAG_MANAGER_HPP_
#define LIBPMG_TAG_MANAGER_HPP_

#include <unordered_map>
#include <vector>

#include "tag.hpp"
#include "taggable.hpp"

namespace libpmg {
    
    /**
     A singleton class that manager the definition of every tag, and which tiles they are assigned to.
     It holds an unordered map with a list of Taggable objects for every existing Tag.
     */
    class TagManager {
        
    public:
        /**
         Use this to access the class.
         @return A singleton reference to this class
         */
        static TagManager& GetInstance() {
            static TagManager instance;
            return instance;
        }
        
        TagManager(TagManager const&) = delete;
        void operator=(TagManager const&) = delete;
        
        /**
         Attempts to add a Taggable reference to a Tag list.
         The same Tag list can't have multiple references to the same object.
         @param taggable A reference to the Taggable object
         @param tag The Tag list to add the Taggable to
         @return True if the Taggable was succesfully added, false otherwise
         */
        bool TryAddTaggable(Taggable *taggable, std::shared_ptr<Tag> tag);
        
        /**
         Remove a specified Taggable object from a Tag list.
         @param taggable A reference to the Taggable object
         @param tag The Tag list from which the Taggable object is to be removed
         */
        void RemoveTaggable(Taggable *taggable, std::shared_ptr<Tag> tag);
        
        std::shared_ptr<Tag> floor_tag_;     /**< A tag indicating the tile has a floor */
        std::shared_ptr<Tag> wall_tag_;      /**< A tag indicating the tile has a wall */
        std::shared_ptr<Tag> door_tag_;      /**< A tag indicating the tile has a door */
        std::shared_ptr<Tag> explored_tag_;  /**< A tag indicating the tile is explored */
        
    private:
        TagManager();
        
        std::unordered_map<std::shared_ptr<Tag>, std::vector<Taggable*>> tag_map_;      /**< An unordered map with a list of Taggable objects */
    };
    
}

#endif /* LIBPMG_TAG_MANAGER_HPP_ */

