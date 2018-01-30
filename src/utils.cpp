#include "utils.hpp"

#include "map.hpp"
#include "priority_queue.hpp"

namespace libpmg {
    
typedef std::shared_ptr<Location> Location_p;
typedef std::shared_ptr<std::unordered_map<Location_p, Location_p>> LocationMap_p;
typedef std::shared_ptr<Map> Map_p;

float heuristic(Location_p loc1, Location_p loc2) {
    int x1, y1, x2, y2;
    std::tie (x1, y1) = loc1->GetXY();
    std::tie (x2, y2) = loc2->GetXY();
    return abs(x1 - x2) + abs(y1 - y2);
}

LocationMap_p Utils::Astar(std::pair<size_t, size_t> start_coor,
                           std::pair<size_t, size_t> end_coor,
                           Map_p map,
                           MoveDirections dir,
                           bool reset_path_flags) {
    if (reset_path_flags)
        map->ResetPathFlags();
    
    auto start_tile {map->GetTile(start_coor)};
    auto end_tile {map->GetTile(end_coor)};
    
    PriorityQueue<Location_p, float> frontier;
    std::unordered_map<Location_p, float> cost_so_far;
    auto came_from {std::make_unique<std::unordered_map<Location_p, Location_p>>()};
    
    //Start point
    start_tile->is_explored = true;
    cost_so_far[start_tile] = start_tile->cost;
    frontier.push(start_tile, start_tile->cost);
    
    while (!frontier.empty()) {
        auto current {frontier.pop()};
        
        for (auto const &nei : map->GetNeighbors(current, dir)) {
            if (map->GetTile(nei->GetXY())->is_explored == false) {
                auto new_cost {cost_so_far[current] + nei->cost};
                if (!cost_so_far.count(nei) || new_cost < cost_so_far[nei]) {
                    cost_so_far[nei] = new_cost;
                    float priority {new_cost + heuristic(nei, end_tile)};
                    frontier.push(nei, priority);
                    (*came_from)[nei] = current;
                    map->GetTile(nei->GetXY())->is_explored = true;
                }
                
                if ((Location_p)map->GetTile(nei->GetXY()) == end_tile)
                    return came_from;
            }
        }
    }
    return nullptr;
}

LocationMap_p Utils::Dijkstra(std::pair<size_t, size_t> start_coor,
                              std::pair<size_t, size_t> end_coor,
                              Map_p map,
                              MoveDirections dir,
                              bool reset_path_flags) {
    if (reset_path_flags)
        map->ResetPathFlags();
    
    auto start_tile {map->GetTile(start_coor)};
    auto end_tile {map->GetTile(end_coor)};
    
    PriorityQueue<Location_p, float> frontier;
    std::unordered_map<Location_p, float> cost_so_far;
    auto came_from {std::make_unique<std::unordered_map<Location_p, Location_p>>()};
    
    //Start point
    start_tile->is_explored = true;
    cost_so_far[start_tile] = start_tile->cost;
    frontier.push(start_tile, start_tile->cost);
    
    while (!frontier.empty()) {
        auto current {frontier.pop()};
        
        for (auto const &nei : map->GetNeighbors(current, dir)) {
            if (map->GetTile(nei->GetXY())->is_explored == false) {
                auto new_cost {cost_so_far[current] + nei->cost};
                if (!cost_so_far.count(nei) || new_cost < cost_so_far[nei]) {
                    cost_so_far[nei] = new_cost;
                    frontier.push(nei, new_cost);
                    (*came_from)[nei] = current;
                    map->GetTile(nei->GetXY())->is_explored = true;
                }
                
                if ((Location_p)map->GetTile(nei->GetXY()) == end_tile)
                    return came_from;
            }
        }
    }
    return nullptr;
}

LocationMap_p Utils::BreadthFirstSearch(std::pair<size_t, size_t> start_coor,
                                        std::pair<size_t, size_t> end_coor,
                                        Map_p map,
                                        bool diagonals,
                                        MoveDirections dir,
                                        bool reset_path_flags) {
    if (reset_path_flags)
        map->ResetPathFlags();
    
    auto start_tile {map->GetTile(start_coor)};
    auto end_tile {map->GetTile(end_coor)};
    
    std::queue<Location_p> frontier;
    auto came_from {std::make_unique<std::unordered_map<Location_p, Location_p>>()};
    
    //Start point
    start_tile->is_explored = true;
    frontier.push(start_tile);
    
    while (!frontier.empty()) {
        auto current {frontier.front()};
        auto neis {map->GetNeighbors(current, dir)};
        
        if ((diagonals && dir == MoveDirections::FOUR_DIRECTIONAL) &&
            ((current->GetX() + current->GetY()) % 2 == 0))
            std::reverse(neis.begin(), neis.end());
        
        for (auto const &nei : neis) {
            if (map->GetTile(nei->GetXY())->is_explored == false) {
                frontier.push(nei);
                (*came_from)[nei] = current;
                map->GetTile(nei->GetXY())->is_explored = true;
            }
            
            if ((Location_p)map->GetTile(nei->GetXY()) == end_tile)
                return came_from;
        }
        
        frontier.pop();
    }
    
    return nullptr;
}
    
}
