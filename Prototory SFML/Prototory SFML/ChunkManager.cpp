#include "ChunkManager.h"



ChunkManager::ChunkManager() 
	: m_chunkCountX(0),
	  m_chunkCountY(0),
	  m_seed(0)
{
}

void ChunkManager::initialize(uint32_t t_seed, TileMap& t_tileMap, LoadingScreen* t_loadingScreen)
{
	m_seed = t_seed;

	/*- Determines chunk dimensions based on the map size and chunk size -*/
	m_chunkCountX = Globals::WORLD_WIDTH / Globals::CHUNK_SIZE;
	m_chunkCountY = Globals::WORLD_HEIGHT / Globals::CHUNK_SIZE;

	int l_progress = 0.f;
	int l_chunksCompleted = 0;
	int l_chunksToEval = m_chunkCountX * m_chunkCountY;


	std::cout << "ChunkManager: Generating world with seed: " << m_seed << "\n\n";

	if (t_loadingScreen)
	{
		t_loadingScreen->setTask("Generating Chunks..");
		t_loadingScreen->setProgress(0.0f);
		t_loadingScreen->render();
	}


	m_chunks.resize(m_chunkCountX);

	for (int x = 0; x < m_chunkCountX; x++)
	{
		for (int y = 0; y < m_chunkCountY; y++)
		{
			m_chunks[x].emplace_back(ChunkPosition{ x , y });
		}
	}

	// Generate each chunk and sync to TileMap
	for (int x = 0; x < m_chunkCountX; x++)
	{
		for (int y = 0; y < m_chunkCountY; y++)
		{
			Chunk& l_chunk = m_chunks[x][y];
			m_worldGen.generateChunk(l_chunk, m_seed);

			// Sync chunk tiles into TileMap for rendering
			for (int localX = 0; localX < Globals::CHUNK_SIZE; localX++)
			{
				for (int localY = 0; localY < Globals::CHUNK_SIZE; localY++)
				{
					Tile& l_tile = l_chunk.getTile(localX, localY);
					t_tileMap.setTileAt(l_tile.m_gridPosition.x, l_tile.m_gridPosition.y, l_tile.m_type);
				}

				l_chunksCompleted++;


				int l_currentProgress = (l_chunksCompleted * 100) / l_chunksToEval;

				if (l_currentProgress != l_progress && t_loadingScreen)
				{
					l_progress = l_currentProgress;

					float l_percent = static_cast<float>(l_chunksCompleted) / static_cast<float>(l_chunksToEval);

					t_loadingScreen->setProgress(l_percent);
					t_loadingScreen->render();
				}

			}
		}
	}
}

Chunk& ChunkManager::getChunk(int t_x, int t_y)
{
	if (!isValidChunkPos(t_x, t_y))
	{
		throw std::out_of_range("Chunk is out of Evalutated Chunk Grid.\n\n");
	}

	return m_chunks[t_x][t_y];
}

bool ChunkManager::isValidChunkPos(int t_x, int t_y) const
{
	return t_x >= 0 && t_x < m_chunkCountX &&
		   t_y >= 0 && t_y < m_chunkCountY;
}


