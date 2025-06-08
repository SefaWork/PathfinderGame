#ifndef TILE_H
#define TILE_H

enum class TileType {
    FLOOR,
    WALL,
    WATER,
    SWAMP,
    EXIT
};

class Tile {
public:
    TileType type;

    Tile(TileType type, int movementCost, bool traversable);

    [[nodiscard]] int getMovementCost() const {return movementCost;}
    [[nodiscard]] bool isTraversable() const {return traversable;}
private:
    int movementCost;
    bool traversable;
};

#endif //TILE_H
