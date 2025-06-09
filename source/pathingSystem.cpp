#include "../include/pathfindingSystem.h"
#include <queue>
#include <vector>
#include <iostream>  // Debug için

#include "../include/pathfinding/astarPathfinder.h"
#include "../include/pathfinding/bfsPathfinder.h"
#include "../include/pathfinding/dijkstraPathfinder.h"

PathfindingSystem::PathfindingSystem() {
    astarPathfinder = new AStarPathfinder();
    bfsPathfinder = new BFSPathfinder();
    dijkstraPathfinder = new DijkstraPathfinder();
};

PathfindingSystem::~PathfindingSystem() {
    delete astarPathfinder;
    delete bfsPathfinder;
    delete dijkstraPathfinder;
}

Path PathfindingSystem::findPath(const SDL_Point& start, const SDL_Point& end, const Map& map, AlgorithmType algorithm) {
    switch (algorithm) {
        case AlgorithmType::BFS:
            return bfsPathfinder->findPath(start, end, map);
        case AlgorithmType::DIJKSTRA:
            return dijkstraPathfinder->findPath(start, end, map);
        case AlgorithmType::ASTAR:
            return astarPathfinder->findPath(start, end, map);
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