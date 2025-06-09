#include "../include/pathfinder.h"

#include <algorithm>
#include <iostream>

int Pathfinder::heuristic(const SDL_Point &a, const SDL_Point &b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

Path Pathfinder::reconstructPath(const SDL_Point &start, const SDL_Point &end, const Map &map, const std::vector<std::vector<SDL_Point> > &cameFrom) {
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

