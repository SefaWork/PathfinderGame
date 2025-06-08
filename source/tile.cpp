#include "../include/tile.h"

Tile::Tile(TileType type, int cost, bool traversable) {
    this->type = type;
    this->movementCost = cost;
    this->traversable = traversable;
}

