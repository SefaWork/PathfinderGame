#include "../../include/pathfinding/bfsPathfinder.h"
#include <queue>

Path BFSPathfinder::findPath(const SDL_Point &start, const SDL_Point &end, const Map &map) {
    int mapWidth = map.getWidth();
    int mapHeight = map.getHeight();
    if (start.x < 0 || start.x >= mapWidth || start.y < 0 || start.y >= mapHeight ||
        end.x < 0 || end.x >= mapWidth || end.y < 0 || end.y >= mapHeight ||
        !map.getTile(start.x, start.y).isTraversable() ||
        !map.getTile(end.x, end.y).isTraversable()) {
        return {};
        }
    if (start.x == end.x && start.y == end.y) return {start};

    std::queue<SDL_Point> frontier;
    frontier.push(start);
    std::vector<std::vector<SDL_Point>> cameFrom(mapHeight, std::vector<SDL_Point>(mapWidth, {-1, -1}));
    std::vector<std::vector<bool>> visited(mapHeight, std::vector<bool>(mapWidth, false));
    visited[start.y][start.x] = true;
    bool pathFound = false;
    SDL_Point directions[] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    while (!frontier.empty()) {
        SDL_Point current = frontier.front(); frontier.pop();
        if (current.x == end.x && current.y == end.y) { pathFound = true; break; }
        for (const auto& dir : directions) {
            SDL_Point next = {current.x + dir.x, current.y + dir.y};
            if (next.x >= 0 && next.x < mapWidth && next.y >= 0 && next.y < mapHeight &&
                !visited[next.y][next.x] && map.getTile(next.x, next.y).isTraversable()) {
                frontier.push(next);
                visited[next.y][next.x] = true;
                cameFrom[next.y][next.x] = current;
                }
        }
    }
    if (pathFound) return reconstructPath(start, end, map, cameFrom);
    return {};
}
