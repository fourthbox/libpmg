#ifndef LIBPMG_UTILS_HPP_
#define LIBPMG_UTILS_HPP_

#include <queue>
#include <unordered_map>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "map.hpp"

namespace libpmg {
    
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

struct Utils {
    static inline void LogDebug(std::string const &message) {
        
        printf("%s\n", message.c_str());
    }
    
    static inline void LogDebug(std::string const &context, std::string const &message) {
        
        printf("%s:: %s\n", context.c_str(), message.c_str());
    }
    
    static inline void LogWarning(std::string const &context, std::string const &message) {
        
        printf("[WARNING] %s:: %s\n", context.c_str(), message.c_str());
    }
    
    static inline void LogError(std::string const &context, std::string const &message) {
        
        printf("[ERROR] %s:: %s\n", context.c_str(), message.c_str());
    }
    
    static std::shared_ptr<std::unordered_map<std::shared_ptr<Location>, std::shared_ptr<Location>>>
    BreadthFirstSearch(std::pair<std::size_t, std::size_t> start_coor,
                       std::pair<std::size_t, std::size_t> end_coor,
                       std::shared_ptr<Map> map,
                       bool diagonals,
                       MoveDirections dir,
                       bool reset_path_flags = true);
    
    static std::shared_ptr<std::unordered_map<std::shared_ptr<Location>, std::shared_ptr<Location>>>
    Dijkstra(std::pair<std::size_t, std::size_t> start_coor,
             std::pair<std::size_t, std::size_t> end_coor,
             std::shared_ptr<Map> map,
             MoveDirections dir,
             bool reset_path_flags = true);
    
    static std::shared_ptr<std::unordered_map<std::shared_ptr<Location>, std::shared_ptr<Location>>>
    Astar(std::pair<std::size_t, std::size_t> start_coor,
          std::pair<std::size_t, std::size_t> end_coor,
          std::shared_ptr<Map> map,
          MoveDirections dir,
          bool reset_path_flags = true);
    
    static std::string GenerateUUID() { return boost::uuids::to_string(boost::uuids::random_generator()()); }
};

}

#endif /* LIBPMG_UTILS_HPP_ */
