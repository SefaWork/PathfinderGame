#include "../../include/pathfinding/astarPathfinder.h"

#include <queue>

struct AStarNode {
    SDL_Point point;
    int f_cost;
    bool operator>(const AStarNode& other) const { return f_cost > other.f_cost; }
};

Path AStarPathfinder::findPath(const SDL_Point &start, const SDL_Point &end, const Map &map) {
    int mapWidth = map.getWidth();
    int mapHeight = map.getHeight();
    if (start.x < 0 || start.x >= mapWidth || start.y < 0 || start.y >= mapHeight ||
        end.x < 0 || end.x >= mapWidth || end.y < 0 || end.y >= mapHeight ||
        !map.getTile(start.x, start.y).isTraversable() ||
        !map.getTile(end.x, end.y).isTraversable()) {
        return {};
    }
    if (start.x == end.x && start.y == end.y) return {start};

    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> frontier;
    std::vector<std::vector<int>> g_cost(mapHeight, std::vector<int>(mapWidth, INT_MAX));
    std::vector<std::vector<SDL_Point>> cameFrom(mapHeight, std::vector<SDL_Point>(mapWidth, {-1, -1}));
    g_cost[start.y][start.x] = 0;
    frontier.push({start, g_cost[start.y][start.x] + heuristic(start, end)});
    bool pathFound = false;
    SDL_Point directions[] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    while (!frontier.empty()) {
        AStarNode current_node_astar = frontier.top(); frontier.pop();
        SDL_Point current_point = current_node_astar.point;
        if (current_point.x == end.x && current_point.y == end.y) { pathFound = true; break; }
        for (const auto& dir : directions) {
            SDL_Point next_point = {current_point.x + dir.x, current_point.y + dir.y};
            if (next_point.x >= 0 && next_point.x < mapWidth && next_point.y >= 0 && next_point.y < mapHeight &&
                map.getTile(next_point.x, next_point.y).isTraversable()) {
                int movement_cost_to_next = map.getTile(next_point.x, next_point.y).isTraversable();
                if (movement_cost_to_next <= 0) movement_cost_to_next = 1;
                int tentative_g_cost = g_cost[current_point.y][current_point.x] + movement_cost_to_next;
                if (tentative_g_cost < g_cost[next_point.y][next_point.x]) {
                    cameFrom[next_point.y][next_point.x] = current_point;
                    g_cost[next_point.y][next_point.x] = tentative_g_cost;
                    int f_cost = tentative_g_cost + heuristic(next_point, end);
                    frontier.push({next_point, f_cost});
                }
            }
        }
    }
    if (pathFound) return reconstructPath(start, end, map, cameFrom);
    return {};
}
