#include "Chunk.h"

Chunk::Chunk(ChunkPosition t_position)
	: m_position(t_position)
	, m_biomeType(BiomeType::Unassigned)
{
	m_tiles.resize(Globals::CHUNK_SIZE, std::vector<Tile>(Globals::CHUNK_SIZE));
}

Tile& Chunk::getTile(int t_x, int t_y)
{
	return m_tiles[t_x][t_y];
}
