#include "../include/pathfindingSystem.h"
#include <queue>
#include <vector>
#include <algorithm> // std::reverse için
#include <iostream>  // Debug için
#include <climits>    // INT_MAX için
#include <cmath>      // std::abs için (heuristic)

struct DijkstraNode {
    SDL_Point point;
    int cost;
    bool operator>(const DijkstraNode& other) const { return cost > other.cost; }
};

struct AStarNode {
    SDL_Point point;
    int f_cost;
    bool operator>(const AStarNode& other) const { return f_cost > other.f_cost; }
};

PathfindingSystem::PathfindingSystem() = default;

Path PathfindingSystem::findPath(const SDL_Point& start, const SDL_Point& end, const Map& map, AlgorithmType algorithm) {
    switch (algorithm) {
        case AlgorithmType::BFS:
            return findPathBFS(start, end, map);
        case AlgorithmType::DIJKSTRA:
            return findPathDijkstra(start, end, map);
        case AlgorithmType::ASTAR:
            return findPathAStar(start, end, map);
        case AlgorithmType::WAVEFRONT:
             std::cerr << "Use generateWavefrontMap for Wavefront map generation." << std::endl;
             break;
    }
    return {};
}

WavefrontMap PathfindingSystem::generateWavefrontMap(const SDL_Point& target, const Map& map) {
    int mapWidth = map.getWidth();
    int mapHeight = map.getHeight();
    WavefrontMap waveMap(mapHeight, std::vector<int>(mapWidth, -1));

    if (target.x < 0 || target.x >= mapWidth || target.y < 0 || target.y >= mapHeight ||
        !map.getTile(target.x, target.y).isTraversable()) {
        std::cerr << "Wavefront: Invalid target point or non-traversable." << std::endl;
        return waveMap;
    }

    std::queue<SDL_Point> frontier;
    frontier.push(target);
    waveMap[target.y][target.x] = 0;
    SDL_Point directions[] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    while (!frontier.empty()) {
        SDL_Point current = frontier.front();
        frontier.pop();
        int currentWaveValue = waveMap[current.y][current.x];
        for (const auto& dir : directions) {
            SDL_Point neighbor = {current.x + dir.x, current.y + dir.y};
            if (neighbor.x >= 0 && neighbor.x < mapWidth &&
                neighbor.y >= 0 && neighbor.y < mapHeight &&
                map.getTile(neighbor.x, neighbor.y).isTraversable() &&
                waveMap[neighbor.y][neighbor.x] == -1) {
                waveMap[neighbor.y][neighbor.x] = currentWaveValue + 1;
                frontier.push(neighbor);
            }
        }
    }
    std::cout << "Wavefront map generated for target (" << target.x << "," << target.y << ")" << std::endl;
    return waveMap;
}

Path PathfindingSystem::findPathBFS(const SDL_Point& start, const SDL_Point& end, const Map& map) {
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

Path PathfindingSystem::findPathDijkstra(const SDL_Point& start, const SDL_Point& end, const Map& map) {
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

int PathfindingSystem::heuristic(const SDL_Point& a, const SDL_Point& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

Path PathfindingSystem::findPathAStar(const SDL_Point& start, const SDL_Point& end, const Map& map) {
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

Path PathfindingSystem::reconstructPath(const SDL_Point& start, const SDL_Point& end,
                                         const Map& map,
                                         const std::vector<std::vector<SDL_Point>>& cameFrom) {
    Path path;
    SDL_Point current = end;
    int iteration_count = 0;
    const int MAX_ITERATIONS = map.getWidth() * map.getHeight() * 2;

    while (!(current.x == start.x && current.y == start.y)) {
        iteration_count++;
        if (iteration_count > MAX_ITERATIONS) {
            std::cerr << "reconstructPath: Exceeded MAX_ITERATIONS. Aborting." << std::endl;
            return {};
        }
        path.push_back(current);
        if (current.y < 0 || static_cast<size_t>(current.y) >= cameFrom.size() || current.x < 0 || static_cast<size_t>(current.x) >= cameFrom[0].size()) {
             std::cerr << "reconstructPath: Current (" << current.x << "," << current.y << ") is out of cameFrom bounds!" << std::endl;
             return {};
        }
        SDL_Point prev = cameFrom[current.y][current.x];
        if (prev.x == -1 && prev.y == -1) {
            std::cerr << "Error reconstructing path: cameFrom link broken for point ("
                      << current.x << "," << current.y << ")." << std::endl;
            return {};
        }
        current = prev;
        if (path.size() > static_cast<size_t>(map.getWidth() * map.getHeight())) {
             std::cerr << "Error reconstructing path: Path too long." << std::endl;
             return {};
        }
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}