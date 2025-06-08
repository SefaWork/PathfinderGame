#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <SDL.h>

#include "tile.h"

class Map {
public:
    Map();
    bool loadMap(const std::string& filePath, int tilePixelSize);
    void draw(SDL_Renderer* renderer);
    [[nodiscard]] const Tile& getTile(int x, int y) const;
    [[nodiscard]] SDL_Point getExitPosition() const;
    [[nodiscard]] int getWidth() const {return width;}
    [[nodiscard]] int getHeight() const {return height;}
    [[nodiscard]] int getTileSize() const {return tileSize;}
private:
    std::vector<std::vector<Tile>> tiles;
    int width;
    int height;
    int tileSize;
    SDL_Point exitPosition;
};

#endif //MAP_H
