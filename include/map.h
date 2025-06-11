#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <SDL.h>

#include "tile.h"

/// Holds and handles current game map.
class Map {
public:
    Map();

    /// Loads a new map from given filepath.
    /// @param filePath File to load.
    /// @param tilePixelSize How many pixels each tile should be.
    /// @return TRUE if successful, FALSE if errored.
    bool loadMap(const std::string& filePath, int tilePixelSize);

    /// Draws all tiles.
    /// @param renderer Renderer pointer.
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
