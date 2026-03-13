#pragma once

#include "Chunk.h"
#include "Globals.h"
#include "TileMap.h"
#include "LoadingScreen.h"
#include "WorldGenerator.h"
#include <stdexcept>	


class ChunkManager
{
public:
	ChunkManager();


	// Utilizes seed set by Game to generate the Chunk Grid
	void initialize(uint32_t t_seed, TileMap& t_tileMap, LoadingScreen* t_loadingScreen);


	Chunk& getChunk(int t_x, int t_y);

	bool isValidChunkPos(int t_x, int t_y) const;

	int getChunkWidth() { return m_chunkCountX; }
	int getChunkHeight() { return m_chunkCountY; }

private:
	std::vector<std::vector<Chunk>> m_chunks;
	WorldGenerator m_worldGen;
	int m_chunkCountX;	int m_chunkCountY;
	uint32_t m_seed;


};