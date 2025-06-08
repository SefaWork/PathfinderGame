#include "../include/map.h"
#include <fstream>
#include <iostream>
#include <SDL.h>
#include <vector>

Map::Map(): exitPosition({-1, -1}) {
    this->width = 0;
    this->height = 0;
    this->tileSize = 32;
    this->tiles = std::vector<std::vector<Tile>>();
}

bool Map::loadMap(const std::string& filePath, int tilePixelSize) {
    this->tileSize = tilePixelSize;
    std::ifstream mapFile(filePath);

    if (!mapFile.is_open()) {
        std::cerr << "Error: Could not open map file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    int currentHeight = 0;
    tiles.clear();
    exitPosition = {-1, -1};

    while (std::getline(mapFile, line)) {
        if (line.empty()) continue;

        if (currentHeight == 0) {
            width = line.length();
        } else if (line.length() != static_cast<size_t>(width)) {
            std::cerr << "Error: Map rows have inconsistent widths in file: " << filePath << std::endl;
            std::cerr << "Expected width: " << width << ", Row " << currentHeight << " width: " << line.length() << std::endl;
            mapFile.close();
            return false;
        }

        std::vector<Tile> rowTiles;
        for (int x = 0; x < width; ++x) {
            switch (line[x]) {
                case 'W':
                    rowTiles.emplace_back(TileType::WALL, 0, false);
                    break;
                case 'F':
                    rowTiles.emplace_back(TileType::FLOOR, 1, true);
                    break;
                case 'S':
                    rowTiles.emplace_back(TileType::WATER, 5, true);
                    break;
                case 'E':
                    rowTiles.emplace_back(TileType::EXIT, 1, true);
                    if (exitPosition.x != -1) {
                        std::cerr << "Warning: Multiple exit points ('E') found in map. Using the last one at ("
                                  << x << "," << currentHeight << ")" << std::endl;
                    }
                    exitPosition = SDL_Point{x, currentHeight};
                    break;
                default:
                    rowTiles.emplace_back(TileType::FLOOR, 1, true);
                    break;
            }
        }
        tiles.push_back(rowTiles);
        currentHeight++;
    }

    height = currentHeight;
    mapFile.close();

    if (width == 0 || height == 0) {
        std::cerr << "Error: Map is empty or could not be loaded correctly from " << filePath << std::endl;
        return false;
    }
    if (exitPosition.x == -1 || exitPosition.y == -1) {
        std::cout << "Warning: No exit point ('E') defined in map file: " << filePath << std::endl;
    }

    std::cout << "Map loaded: " << filePath << " (" << width << "x" << height << " tiles). Exit at: ("
              << exitPosition.x << "," << exitPosition.y << ")" << std::endl;
    return true;
}

void Map::draw(SDL_Renderer* renderer) {
    if (renderer == nullptr) return;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (y >= tiles.size() || x >= tiles[y].size()) {
                std::cerr << "Error in Map::draw: Accessing out of bounds tile at (" << x << "," << y << ")" << std::endl;
                continue;
            }
            const Tile& tile = tiles[y][x];
            SDL_Rect destRect = {
                x * tileSize,
                y * tileSize,
                tileSize,
                tileSize
            };

            switch (tile.type) {
                case TileType::FLOOR:
                    SDL_SetRenderDrawColor(renderer, 50, 150, 50, 255);
                    break;
                case TileType::WALL:
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                    break;
                case TileType::WATER:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
                    break;
                case TileType::SWAMP:
                     SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
                     break;
                case TileType::EXIT:
                    SDL_SetRenderDrawColor(renderer, 200, 200, 0, 255);
                    break;
                default:
                    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
                    break;
            }
            SDL_RenderFillRect(renderer, &destRect);
        }
    }
}

SDL_Point Map::getExitPosition() const {
    return exitPosition;
}

const Tile& Map::getTile(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        if (static_cast<size_t>(y) < tiles.size() && static_cast<size_t>(x) < tiles[y].size()) {
             return tiles[y][x];
        }
    }

    static Tile errorTile(TileType::WALL, 0, false);
    std::cerr << "Error: Accessing tile out of bounds (" << x << "," << y << "). Returning error tile." << std::endl;
    return errorTile;
}
