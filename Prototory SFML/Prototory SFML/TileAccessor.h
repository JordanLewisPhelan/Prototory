#pragma once

#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Globals.h"


class ChunkManager;

class TileAccessor
{
public:
    TileAccessor(ChunkManager& t_chunkManager);

    // Returns tile at world grid position, nullptr if out of bounds
    Tile* get(sf::Vector2i t_gridPos);

    // Returns true if world grid position is within world bounds
    bool  isValid(sf::Vector2i t_gridPos) const;

private:
    ChunkManager& m_chunkManager;
};