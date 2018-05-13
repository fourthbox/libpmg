#include "utils.hpp"

#include "map.hpp"

namespace libpmg {
    
typedef std::unique_ptr<std::unordered_map<Location*, Location*>> LocationMap_up;

LocationMap_up Utils::Astar(std::pair<size_t, size_t> start_coor,
                           std::pair<size_t, size_t> end_coor,
                           Map *map,
                           MoveDirections const &dir,
                           bool reset_path_flags) {
    if (reset_path_flags)
        map->ResetPathFlags();
    
    auto start_tile {map->GetTile(start_coor)};
    auto end_tile {map->GetTile(end_coor)};
    
    PriorityQueue<Location*, float> frontier;
    std::unordered_map<Location*, float> cost_so_far;
    auto came_from {std::make_unique<std::unordered_map<Location*, Location*>>()};
    
    //Start point
    start_tile->is_path_explored_ = true;
    cost_so_far[start_tile] = start_tile->path_cost_;
    frontier.push(start_tile, start_tile->path_cost_);
    
    // Calculate heuristic distance
    auto heuristic_distance_calc = [=] (Location *loc1, Location *loc2) -> float {
        int x1, y1, x2, y2;
        std::tie (x1, y1) = loc1->GetXY();
        std::tie (x2, y2) = loc2->GetXY();
        return abs(x1 - x2) + abs(y1 - y2);
    };
    
    while (!frontier.empty()) {
        auto current {frontier.pop()};
        
        for (auto const &nei : map->GetNeighbors(current, dir)) {
            if (nei->is_path_explored_ == false) {
                auto new_cost {cost_so_far[current] + nei->path_cost_};
                if (!cost_so_far.count(nei) || new_cost < cost_so_far[nei]) {
                    cost_so_far[nei] = new_cost;
                    
                    // Add heuristic distance calc
                    float priority {new_cost + heuristic_distance_calc(nei, end_tile)};

                    frontier.push(nei, priority);
                    (*came_from)[nei] = current;
                    nei->is_path_explored_ = true;
                }
                
                if ((Location*)map->GetTile(nei->GetXY()) == end_tile)
                    return came_from;
            }
        }
    }
    return nullptr;
}

LocationMap_up Utils::Dijkstra(std::pair<size_t, size_t> start_coor,
                              std::pair<size_t, size_t> end_coor,
                              Map *map,
                              MoveDirections const &dir,
                              bool reset_path_flags) {
    if (reset_path_flags)
        map->ResetPathFlags();
    
    auto start_tile {map->GetTile(start_coor)};
    auto end_tile {map->GetTile(end_coor)};
    
    PriorityQueue<Location*, float> frontier;
    std::unordered_map<Location*, float> cost_so_far;
    auto came_from {std::make_unique<std::unordered_map<Location*, Location*>>()};
    
    //Start point
    start_tile->is_path_explored_ = true;
    cost_so_far[start_tile] = start_tile->path_cost_;
    frontier.push(start_tile, start_tile->path_cost_);
    
    while (!frontier.empty()) {
        auto current {frontier.pop()};
        
        for (auto const &nei : map->GetNeighbors(current, dir)) {
            if (nei->is_path_explored_ == false) {
                auto new_cost {cost_so_far[current] + nei->path_cost_};
                if (!cost_so_far.count(nei) || new_cost < cost_so_far[nei]) {
                    cost_so_far[nei] = new_cost;
                    frontier.push(nei, new_cost);
                    (*came_from)[nei] = current;
                    nei->is_path_explored_ = true;
                }
                
                if ((Location*)map->GetTile(nei->GetXY()) == end_tile)
                    return came_from;
            }
        }
    }
    return nullptr;
}

LocationMap_up Utils::BreadthFirstSearch(std::pair<size_t, size_t> start_coor,
                                        std::pair<size_t, size_t> end_coor,
                                        Map *map,
                                        bool diagonals,
                                        MoveDirections const &dir,
                                        bool reset_path_flags) {
    if (reset_path_flags)
        map->ResetPathFlags();
    
    auto start_tile {map->GetTile(start_coor)};
    auto end_tile {map->GetTile(end_coor)};
    
    std::queue<Location*> frontier;
    auto came_from {std::make_unique<std::unordered_map<Location*, Location*>>()};
    
    //Start point
    start_tile->is_path_explored_ = true;
    frontier.push(start_tile);
    
    while (!frontier.empty()) {
        auto current {frontier.front()};
        auto neis {map->GetNeighbors(current, dir)};
        
        if ((diagonals && dir == MoveDirections::FOUR_DIRECTIONAL) &&
            ((current->GetX() + current->GetY()) % 2 == 0))
            std::reverse(neis.begin(), neis.end());
        
        for (auto const &nei : neis) {
            if (nei->is_path_explored_ == false) {
                frontier.push(nei);
                (*came_from)[nei] = current;
                nei->is_path_explored_ = true;
            }
            
            if ((Location*)map->GetTile(nei->GetXY()) == end_tile)
                return came_from;
        }
        
        frontier.pop();
    }
    
    return nullptr;
}
    
}
