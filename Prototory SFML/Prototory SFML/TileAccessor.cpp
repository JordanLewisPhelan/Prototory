	#include "TileAccessor.h"
#include "ChunkManager.h"

TileAccessor::TileAccessor(ChunkManager& t_chunkManager)
	: m_chunkManager(t_chunkManager)
{
}

Tile* TileAccessor::get(sf::Vector2i t_gridPos)
{
	if (!isValid(t_gridPos))
		return nullptr;

	int l_chunkX = t_gridPos.x / Globals::CHUNK_SIZE;
	int l_chunkY = t_gridPos.y / Globals::CHUNK_SIZE;
	int l_localX = t_gridPos.x % Globals::CHUNK_SIZE;
	int l_localY = t_gridPos.y % Globals::CHUNK_SIZE;

	Tile& tile = m_chunkManager.getChunk(
		l_chunkX, l_chunkY).getTile(
			l_localX, l_localY);

	return &tile;
}

bool TileAccessor::isValid(sf::Vector2i t_gridPos) const
{
	int l_chunkX = t_gridPos.x / Globals::CHUNK_SIZE;
	int l_chunkY = t_gridPos.y / Globals::CHUNK_SIZE;	

	return m_chunkManager.isValidChunkPos(l_chunkX, l_chunkY);
}
