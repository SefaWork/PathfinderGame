#include "../../include/pathfinding/dijkstraPathfinder.h"
#include <queue>

struct DijkstraNode {
    SDL_Point point;
    int cost;
    bool operator>(const DijkstraNode& other) const { return cost > other.cost; }
};

Path DijkstraPathfinder::findPath(const SDL_Point &start, const SDL_Point &end, const Map &map) {
    int mapWidth = map.getWidth();
    int mapHeight = map.getHeight();
    if (start.x < 0 || start.x >= mapWidth || start.y < 0 || start.y >= mapHeight ||
        end.x < 0 || end.x >= mapWidth || end.y < 0 || end.y >= mapHeight ||
        !map.getTile(start.x, start.y).isTraversable() ||
        !map.getTile(end.x, end.y).isTraversable()) {
        return {};
    }
    if (start.x == end.x && start.y == end.y) return {start};

    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, std::greater<DijkstraNode>> frontier;
    std::vector<std::vector<int>> costSoFar(mapHeight, std::vector<int>(mapWidth, INT_MAX));
    std::vector<std::vector<SDL_Point>> cameFrom(mapHeight, std::vector<SDL_Point>(mapWidth, {-1, -1}));
    frontier.push({start, 0});
    costSoFar[start.y][start.x] = 0;
    bool pathFound = false;
    SDL_Point directions[] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    while (!frontier.empty()) {
        DijkstraNode current_node = frontier.top(); frontier.pop();
        SDL_Point current_point = current_node.point;
        if (current_node.cost > costSoFar[current_point.y][current_point.x]) continue;
        if (current_point.x == end.x && current_point.y == end.y) { pathFound = true; break; }
        for (const auto& dir : directions) {
            SDL_Point next_point = {current_point.x + dir.x, current_point.y + dir.y};
            if (next_point.x >= 0 && next_point.x < mapWidth && next_point.y >= 0 && next_point.y < mapHeight &&
                map.getTile(next_point.x, next_point.y).isTraversable()) {
                int movement_cost_to_next = map.getTile(next_point.x, next_point.y).getMovementCost();
                if (movement_cost_to_next <= 0) movement_cost_to_next = 1;
                int new_cost = costSoFar[current_point.y][current_point.x] + movement_cost_to_next;
                if (new_cost < costSoFar[next_point.y][next_point.x]) {
                    costSoFar[next_point.y][next_point.x] = new_cost;
                    frontier.push({next_point, new_cost});
                    cameFrom[next_point.y][next_point.x] = current_point;
                }
            }
        }
    }
    if (pathFound) return reconstructPath(start, end, map, cameFrom);
    return {};
}
