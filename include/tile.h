#ifndef TILE_H
#define TILE_H

enum class TileType {
    FLOOR,
    WALL,
    WATER,
    SWAMP,
    EXIT
};

/// Class that holds tile information.
class Tile {
public:
    TileType type;

    Tile(TileType type, int movementCost, bool traversable);

    /// @return Cost of movement.
    [[nodiscard]] int getMovementCost() const {return movementCost;}

    /// @return TRUE if players and enemies can move onto this tile, FALSE if they cannot.
    [[nodiscard]] bool isTraversable() const {return traversable;}
private:
    int movementCost;
    bool traversable;
};

#endif //TILE_H
