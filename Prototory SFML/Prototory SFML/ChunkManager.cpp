#include "ChunkManager.h"



ChunkManager::ChunkManager() 
	: m_chunkCountX(0),
	  m_chunkCountY(0),
	  m_seed(0)
{
}


/// ToDo: Refactor Passes into functions later or during downtime - very large Initialize function, no good
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

	// First pass - Biome Setup and Assignments

	if (t_loadingScreen)
	{
		t_loadingScreen->setTask("Assigning Biomes...");
		t_loadingScreen->setProgress(0.0f);
		t_loadingScreen->render();
	}

	for (int x = 0; x < m_chunkCountX; x++)
	{
		for (int y = 0; y < m_chunkCountY; y++)
		{
			float BIOME_SAMPLE_SCALE = 127.3f;

			float l_biomeValue = m_worldGen.getBiomeValue(
				static_cast<int>(x * BIOME_SAMPLE_SCALE),
				static_cast<int>(y * BIOME_SAMPLE_SCALE),
				t_seed
			);
			m_chunks[x][y].setBiomeType(determineBiomeType(l_biomeValue));

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


	// Second pass - Tile Sync & Generation 
	l_chunksCompleted = 0;
	l_progress = 0;

	if (t_loadingScreen)
	{
		t_loadingScreen->setTask("Generating Terrain...");
		t_loadingScreen->setProgress(0.0f);
		t_loadingScreen->render();
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
					t_tileMap.setTileAt(l_tile.m_gridPosition.x, l_tile.m_gridPosition.y, l_tile.m_type, l_tile.m_elevation);
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

	// Third Pass - Adjusting and Assigning and correcting Tile Elevation
	if (t_loadingScreen)
	{
		t_loadingScreen->setTask("Elevation Alignment...");
		t_loadingScreen->setProgress(0.0f);
		t_loadingScreen->render();
	}

	for (int chunkX = 0; chunkX < m_chunkCountX; chunkX++)
	{
		for (int chunkY = 0; chunkY < m_chunkCountY; chunkY++)
		{
			Chunk& l_chunk = m_chunks[chunkX][chunkY];

			// Determines depth logic for water in Oceans so colour gradienting works
			// Currently unused in other biomes like this
			if (l_chunk.getBiome() != BiomeType::Ocean)
				continue;

			for (int localX = 0; localX < Globals::CHUNK_SIZE; localX++)
			{
				for (int localY = 0; localY < Globals::CHUNK_SIZE; localY++)
				{
					Tile& l_tile = l_chunk.getTile(localX, localY);

					if (l_tile.m_type != TileType::Water)
						continue;

					// Check all 8 neighbours — count how many are also water
					int l_waterNeighbours = 0;

					for (int dx = -1; dx <= 1; dx++)
					{
						for (int dy = -1; dy <= 1; dy++)
						{
							if (dx == 0 && dy == 0)
								continue;

							int l_worldX = l_tile.m_gridPosition.x + dx;
							int l_worldY = l_tile.m_gridPosition.y + dy;

							// Skip if world position is out of bounds entirely
							if (l_worldX < 0 || l_worldY < 0 ||
								l_worldX >= Globals::WORLD_WIDTH ||
								l_worldY >= Globals::WORLD_HEIGHT)
								continue;

							// Convert world coords back to chunk + local coords
							int l_neighbourChunkX = l_worldX / Globals::CHUNK_SIZE;
							int l_neighbourChunkY = l_worldY / Globals::CHUNK_SIZE;

							if (!isValidChunkPos(l_neighbourChunkX, l_neighbourChunkY))
								continue;

							int l_localX = l_worldX % Globals::CHUNK_SIZE;
							int l_localY = l_worldY % Globals::CHUNK_SIZE;

							Tile& l_neighbour = m_chunks[l_neighbourChunkX][l_neighbourChunkY].getTile(l_localX, l_localY);

							if (l_neighbour.m_type == TileType::Water)
								l_waterNeighbours++;
						}
					}

					// 0 water neighbours = elevation 3 (shallow, near land)
					// 8 water neighbours = elevation 0 (deep, fully enclosed)
					l_tile.m_elevation = OCEAN_MAX_ELEVATION - static_cast<int>((l_waterNeighbours / 8.0f) * static_cast<float>(OCEAN_MAX_ELEVATION));
				}
			}
		}
	}

		// Fourth pass - Biome Borders and Elevation smoothing
	if (t_loadingScreen)
	{
		t_loadingScreen->setTask("Terrain Smoothing...");
		t_loadingScreen->setProgress(0.0f);
		t_loadingScreen->render();
	}

	// 3 is default iterations but can be brought up for alignments or down for performance
	smoothElevationBorders(3);


	// Fifth Pass - Border Blending
	if (t_loadingScreen)
	{
		t_loadingScreen->setTask("Biome Blending...");
		t_loadingScreen->setProgress(0.0f);
		t_loadingScreen->render();
	}

	applyBorderBlending();


	// Sync border blending changes back to TileMap
	for (int chunkX = 0; chunkX < m_chunkCountX; chunkX++)
	{
		for (int chunkY = 0; chunkY < m_chunkCountY; chunkY++)
		{
			Chunk& l_chunk = m_chunks[chunkX][chunkY];
			for (int localX = 0; localX < Globals::CHUNK_SIZE; localX++)
			{
				for (int localY = 0; localY < Globals::CHUNK_SIZE; localY++)
				{
					Tile& l_tile = l_chunk.getTile(localX, localY);
					t_tileMap.setTileAt(
						l_tile.m_gridPosition.x,
						l_tile.m_gridPosition.y,
						l_tile.m_type,
						l_tile.m_elevation
					);
				}
			}
		}
	}

	// Sixth Pass - Assign Tile Resources
	if (t_loadingScreen)
	{
		t_loadingScreen->setTask("Populating Resources...");
		t_loadingScreen->setProgress(0.0f);
		t_loadingScreen->render();
	}

	assignTileResources();

	if (t_loadingScreen)
	{
		t_loadingScreen->setTask("Generation Completed");
		t_loadingScreen->setProgress(1.0f);
		t_loadingScreen->render();

		// Forced sleep to ensure it updates for debugging
		sf::sleep(sf::milliseconds(200));
	}

	std::cout << "ChunkManager: World Generation Finalized. \n\n";
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

BiomeType ChunkManager::determineBiomeType(float t_value) const
{
	if (t_value < 0.2)
		return BiomeType::Ocean;
	else if (t_value < 0.5)
		return BiomeType::Plains;
	else if (t_value < 0.7)
		return BiomeType::Forest;
	else if (t_value < 0.85)
		return BiomeType::Mountains;
	else
		return BiomeType::Desert;	

	return BiomeType();
}

int ChunkManager::getBiomeElevationDelta(BiomeType t_biome) const
{
	// Hard coded values refinement may be in order but not super prevalent
	switch (t_biome)
	{
	case BiomeType::Ocean:      return 1;
	case BiomeType::Plains:     return 3;
	case BiomeType::Forest:     return 4;
	case BiomeType::Mountains:  return 5;
	case BiomeType::Desert:     return 2;
	default:                    return 2;
	}
}

void ChunkManager::smoothElevationBorders(int t_iterations)
{
	// Cardinal influences
	const int dx[] = {0, 0, 1, -1};
	const int dy[] = {1, -1, 0, 0 };

	for (int i = 0; i < t_iterations; i++)
	{
		for (int chunkX = 0; chunkX < m_chunkCountX; chunkX++)
		{
			for (int chunkY = 0; chunkY < m_chunkCountY; chunkY++)
			{
				Chunk& l_chunk = m_chunks[chunkX][chunkY];

				for (int localX = 0; localX < Globals::CHUNK_SIZE; localX++)
				{
					for (int localY = 0; localY < Globals::CHUNK_SIZE; localY++)
					{
						Tile& l_tile = l_chunk.getTile(localX, localY);

						int l_worldX = l_tile.m_gridPosition.x;
						int l_worldY = l_tile.m_gridPosition.y;

						for (int d = 0; d < 4; d++)
						{
							int l_neighbourWorldX = l_worldX + dx[d];
							int l_neighbourWorldY = l_worldY + dy[d];

							// Skip out of bounds
							if (l_neighbourWorldX < 0 || l_neighbourWorldY < 0 ||
								l_neighbourWorldX >= Globals::WORLD_WIDTH ||
								l_neighbourWorldY >= Globals::WORLD_HEIGHT)
								continue;

							int l_neighbourChunkX = l_neighbourWorldX / Globals::CHUNK_SIZE;
							int l_neighbourChunkY = l_neighbourWorldY / Globals::CHUNK_SIZE;
							int l_neighbourLocalX = l_neighbourWorldX % Globals::CHUNK_SIZE;
							int l_neighbourLocalY = l_neighbourWorldY % Globals::CHUNK_SIZE;

							Tile& l_neighbour = m_chunks[l_neighbourChunkX][l_neighbourChunkY]
								.getTile(l_neighbourLocalX, l_neighbourLocalY);

							int l_delta = std::abs(l_tile.m_elevation - l_neighbour.m_elevation);

							// Lower tile's biome rule determines max allowed delta
							BiomeType l_lowerBiome = (l_tile.m_elevation <= l_neighbour.m_elevation)
								? l_chunk.getBiome()
								: m_chunks[l_neighbourChunkX][l_neighbourChunkY].getBiome();

							int l_maxDelta = getBiomeElevationDelta(l_lowerBiome);

							if (l_delta > l_maxDelta)
							{
								// Average both tiles toward each other
								int l_avg = (l_tile.m_elevation + l_neighbour.m_elevation) / 2;
								l_tile.m_elevation = l_avg;
								l_neighbour.m_elevation = l_avg;
							}
						}
					}
				}
			}
		}
	}
}

void ChunkManager::applyBorderBlending()
{
	// All border override rules defined in one place 
	/// ToDo: Expand into full rulings class later on
	/// servicable but very heavy on ChunkManager size
	static const std::vector<BorderRule> BORDER_RULES =
	{
		{ BiomeType::Forest,    BiomeType::Ocean,   TileType::Sand  },
		{ BiomeType::Forest,    BiomeType::Desert,  TileType::Sand  },
		{ BiomeType::Plains,    BiomeType::Ocean,   TileType::Sand  },
		{ BiomeType::Mountains, BiomeType::Plains,  TileType::Grass },
		{ BiomeType::Desert,    BiomeType::Ocean,   TileType::Sand  }
	};

	// Cardinal directions — which edge of the chunk faces which neighbour
	const int dx[] = { 0, 0, 1, -1 };
	const int dy[] = { 1, -1, 0, 0 };

	constexpr int BORDER_DEPTH = 2;

	for (int chunkX = 0; chunkX < m_chunkCountX; chunkX++)
	{
		for (int chunkY = 0; chunkY < m_chunkCountY; chunkY++)
		{
			Chunk& l_chunk = m_chunks[chunkX][chunkY];
			BiomeType l_sourceBiome = l_chunk.getBiome();

			// Check all 4 cardinal neighbours
			for (int d = 0; d < 4; d++)
			{
				int l_neighbourChunkX = chunkX + dx[d];
				int l_neighbourChunkY = chunkY + dy[d];

				if (!isValidChunkPos(l_neighbourChunkX, l_neighbourChunkY))
					continue;

				BiomeType l_neighbourBiome = m_chunks[l_neighbourChunkX][l_neighbourChunkY].getBiome();

				// Find a matching rule for this biome pairing
				const BorderRule* l_rule = nullptr;
				for (const BorderRule& rule : BORDER_RULES)
				{
					if (rule.sourceBiome == l_sourceBiome &&
						rule.neighbourBiome == l_neighbourBiome)
					{
						l_rule = &rule;
						break;
					}
				}

				if (!l_rule)
					continue;

				// Apply override to edge tiles within BORDER_DEPTH
				for (int localX = 0; localX < Globals::CHUNK_SIZE; localX++)
				{
					for (int localY = 0; localY < Globals::CHUNK_SIZE; localY++)
					{
						uint32_t l_SEEDOFFSET = 1457894689u;

						// Determine how close this tile is to the relevant edge
						int l_edgeDist = 0;
						if (dx[d] == 1) l_edgeDist = (Globals::CHUNK_SIZE - 1) - localX;
						if (dx[d] == -1) l_edgeDist = localX;
						if (dy[d] == 1) l_edgeDist = (Globals::CHUNK_SIZE - 1) - localY;
						if (dy[d] == -1) l_edgeDist = localY;

						if (l_edgeDist >= BORDER_DEPTH)
							continue;

						Tile& l_tile = l_chunk.getTile(localX, localY);

						// Use hash noise to vary the border naturally
						// Tiles further from the edge need a higher noise value to be overridden
						// giving an irregular rather than perfectly straight border
						float l_noise = m_worldGen.getBiomeValue(
							l_tile.m_gridPosition.x,
							l_tile.m_gridPosition.y,
							m_seed ^ l_SEEDOFFSET
						);

						// Edge tile (dist 0) — always override
						// One tile in (dist 1) — only override if noise clears threshold
						float l_threshold = (l_edgeDist == 0) ? 0.3f : 0.6f;

						if (l_noise >= l_threshold)
							l_tile.m_type = l_rule->overrideTile;
					}
				}
			}
		}
	}
}

void ChunkManager::assignTileResources()
{
	for (int chunkX = 0; chunkX < m_chunkCountX; chunkX++)
	{
		for (int chunkY = 0; chunkY < m_chunkCountY; chunkY++)
		{
			Chunk& l_chunk = m_chunks[chunkX][chunkY];
			BiomeType l_biome = l_chunk.getBiome();

			for (int localX = 0; localX < Globals::CHUNK_SIZE; localX++)
			{
				for (int localY = 0; localY < Globals::CHUNK_SIZE; localY++)
				{
					Tile& l_tile = l_chunk.getTile(localX, localY);

					/// ToDo: Overhaul to limit resource availability - e.g. Mountains
					/// can only have 6 tiles in a whole chunk - May be a larger refactor 
					/// but its what i want post project to make the land feel more natural or limited
					// Based on tile and biome altered quantities
					switch (l_tile.m_type)
					{
					case TileType::Trees:
						l_tile.m_resource.m_resourceID = 1; // Wood
						l_tile.m_resource.m_maxQuantity = (l_biome == BiomeType::Forest) ? 50 : 25;
						l_tile.m_resource.m_currentQuantity = l_tile.m_resource.m_maxQuantity;
						l_tile.m_resource.m_interactionType = InteractionType::Chop;
						break;

					case TileType::Stone:
						l_tile.m_resource.m_resourceID = 2; // Stone
						l_tile.m_resource.m_maxQuantity = (l_biome == BiomeType::Mountains) ? 80 : 40;
						l_tile.m_resource.m_currentQuantity = l_tile.m_resource.m_maxQuantity;
						l_tile.m_resource.m_interactionType = InteractionType::Mine;
						break;

					case TileType::IronOre:
						l_tile.m_resource.m_resourceID = 3; // Iron Ore
						l_tile.m_resource.m_maxQuantity = (l_biome == BiomeType::Mountains) ? 40 : 15;
						l_tile.m_resource.m_currentQuantity = l_tile.m_resource.m_maxQuantity;
						l_tile.m_resource.m_interactionType = InteractionType::Mine;
						break;

					case TileType::Sand:
						l_tile.m_resource.m_resourceID = 2; // Sand
						l_tile.m_resource.m_maxQuantity = (l_biome == BiomeType::Desert) ? 100 : 60;
						l_tile.m_resource.m_currentQuantity = l_tile.m_resource.m_maxQuantity;
						l_tile.m_resource.m_interactionType = InteractionType::Shovel;
						break;

					case TileType::Grass:
						l_tile.m_resource.m_resourceID = 3; // Grass
						l_tile.m_resource.m_maxQuantity = (l_biome == BiomeType::Plains) ? 60 : 25;
						l_tile.m_resource.m_currentQuantity = l_tile.m_resource.m_maxQuantity;
						l_tile.m_resource.m_interactionType = InteractionType::Shovel;
						break;

					default:
						// Quantity stays 0 == not harvestable
						break;
					}
				}
			}
		}
	}
}


