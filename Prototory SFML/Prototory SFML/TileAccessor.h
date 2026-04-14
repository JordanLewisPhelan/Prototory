#pragma once

#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Globals.h"

class ChunkManager;

class TileAccessor
{
public:
    TileAccessor(ChunkManager& t_chunkManager);

    
    Tile* get(sf::Vector2i t_gridPos);

    
    bool  isValid(sf::Vector2i t_gridPos) const;

private:
    ChunkManager& m_chunkManager;
};