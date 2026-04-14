#pragma once

#include "Tile.h"
#include "Globals.h"
#include <cstdint>
#include <vector>

enum class BiomeType {
	Unassigned, Plains, Forest, Mountains, Ocean, Desert
};

struct ChunkPosition {
	int x;
	int y;
};

class Chunk
{
public:
	Chunk(ChunkPosition t_position);

	
	ChunkPosition getPosition() { return m_position; }
	BiomeType getBiome() { return m_biomeType; }
	std::vector<std::vector<Tile>>& getTiles() { return m_tiles; }

	void setBiomeType(BiomeType t_biome) { m_biomeType = t_biome; }

	Tile& getTile(int t_x, int t_y);

private:
	std::vector<std::vector<Tile>> m_tiles;
	ChunkPosition m_position; 
	BiomeType m_biomeType;

};