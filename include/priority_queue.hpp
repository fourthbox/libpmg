#ifndef LIBPMG_PRIORITY_QUEUE_HPP_
#define LIBPMG_PRIORITY_QUEUE_HPP_

#include <algorithm>
#include <queue>

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
    
}

#endif /* LIBPMG_PRIORITY_QUEUE_HPP_ */
