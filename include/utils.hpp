/**
 @file tile.hpp
 @author pat <pat@fourthbox.com>
 */

#ifndef LIBPMG_UTILS_HPP_
#define LIBPMG_UTILS_HPP_

#include <queue>
#include <unordered_map>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "map.hpp"

namespace libpmg {

/**
 Simple implementation of a priority queue.
 */
template<typename T, typename priority_t>
struct PriorityQueue {
    typedef std::pair<priority_t, T> PQElement;
    std::priority_queue<PQElement, std::vector<PQElement>,
    std::greater<PQElement>> elements;
    
    inline bool empty() const { return elements.empty(); }
    
    inline void push(T item, priority_t priority) {
        elements.emplace(priority, item);
    }
    
    inline T pop() {
        T best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
    
    inline std::size_t size() {
        if (elements.size() < std::numeric_limits<std::size_t>::max())
            return elements.size();
        
        return std::numeric_limits<std::size_t>::max();
    }
};

/**
 A struct containing utility functions
 */
struct Utils {
    
    /**
     Prints debug log informations.
     @param message The message to be printed
     */
    static inline void LogDebug(std::string const &message) {
        
        printf("%s\n", message.c_str());
    }
    
    /**
     Prints debug log informations.
     @param context A string indicating the context for better describing the message
     @param message The message to be printed
     */
    static inline void LogDebug(std::string const &context, std::string const &message) {
        
        printf("%s:: %s\n", context.c_str(), message.c_str());
    }
    
    /**
     Prints debug log informations with a warning sign.
     @param context A string indicating the context for better describing the message
     @param message The message to be printed
     */
    static inline void LogWarning(std::string const &context, std::string const &message) {
        
        printf("[WARNING] %s:: %s\n", context.c_str(), message.c_str());
    }

    /**
     Prints debug log informations with an error sign.
     @param context A string indicating the context for better describing the message
     @param message The message to be printed
     */
    static inline void LogError(std::string const &context, std::string const &message) {
        
        printf("[ERROR] %s:: %s\n", context.c_str(), message.c_str());
    }
    
    /**
     A function that uses the BFS algorithm to find the shortest path between 2 Location on a Map.
     @param start_coor A pair of coordinats representing the start location
     @param end_coor A pair of coordinats representing the end location
     @param map A pointer to the Map where the search is happening
     @param diagonals Whether diagonal paths should be used (compatible with FOUR_DIRECTIONAL, creating a "stair" effect)
     @param dir Whether locations can be connected diagonally
     @param reset_path_flags Whether path flags should be reset before running the algorithm
     @return An unordered map of locations. The key is the location "connected" to the value on the generated path
     */
    static std::shared_ptr<std::unordered_map<Location*, Location*>>
    BreadthFirstSearch(std::pair<std::size_t, std::size_t> start_coor,
                       std::pair<std::size_t, std::size_t> end_coor,
                       Map *map,
                       bool diagonals,
                       MoveDirections const &dir,
                       bool reset_path_flags = true);
    
    /**
     A function that uses the Dijkstra algorithm to find the shortest path between 2 Location on a Map.
     @param start_coor A pair of coordinats representing the start location
     @param end_coor A pair of coordinats representing the end location
     @param map A pointer to the Map where the search is happening
     @param dir Whether locations can be connected diagonally
     @param reset_path_flags Whether path flags should be reset before running the algorithm
     @return An unordered map of locations. The key is the location "connected" to the value on the generated path
     */
    static std::shared_ptr<std::unordered_map<Location*, Location*>>
    Dijkstra(std::pair<std::size_t, std::size_t> start_coor,
             std::pair<std::size_t, std::size_t> end_coor,
             Map *map,
             MoveDirections const &dir,
             bool reset_path_flags = true);
    
    /**
     A function that uses the Astar algorithm to find the shortest path between 2 Location on a Map.
     @param start_coor A pair of coordinats representing the start location
     @param end_coor A pair of coordinats representing the end location
     @param map A pointer to the Map where the search is happening
     @param dir Whether locations can be connected diagonally
     @param reset_path_flags Whether path flags should be reset before running the algorithm
     @return An unordered map of locations. The key is the location "connected" to the value on the generated path
     */
    static std::shared_ptr<std::unordered_map<Location*, Location*>>
    Astar(std::pair<std::size_t, std::size_t> start_coor,
          std::pair<std::size_t, std::size_t> end_coor,
          Map *map,
          MoveDirections const &dir,
          bool reset_path_flags = true);
    
    /**
     Generates a random unique id.
     @return A string with a UUID
     */
    static std::string GenerateUUID() { return boost::uuids::to_string(boost::uuids::random_generator()()); }
};

}

#endif /* LIBPMG_UTILS_HPP_ */
