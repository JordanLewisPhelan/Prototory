#include "ChunkManager.h"


ChunkManager::ChunkManager() 
	: m_chunkCountX(0),
	  m_chunkCountY(0),
	  m_seed(0)
{
}

/// ToDo: Refactor Passes into functions later or during downtime - very large Initialize function, no good
void ChunkManager::initialize(uint32_t t_seed, TileMap& t_tileMap, LoadingScreen* t_loadingScreen, const TileVisuals& t_tileVisuals)
{
	m_seed = t_seed;

	/*- Determines chunk dimensions based on the map size and chunk size -*/
	m_chunkCountX = Globals::WORLD_WIDTH / Globals::CHUNK_SIZE;
	m_chunkCountY = Globals::WORLD_HEIGHT / Globals::CHUNK_SIZE;

	// Updates to world
	buildChunkGrid(t_loadingScreen);
	assignBiomes(t_seed, t_loadingScreen);
	generateTerrain(t_seed, t_tileMap, t_loadingScreen, t_tileVisuals);
	adjustOceanDepth(t_loadingScreen);
	smoothElevation(3, t_loadingScreen);

	// Smoothing / Application assurance
	applyBorderBlending();
	assignTileResources();
	syncToTileMap(t_tileMap);


	if (t_loadingScreen)
	{
		t_loadingScreen->setTask("Generation Completed");
		t_loadingScreen->setProgress(1.0f);
		t_loadingScreen->render();
		sf::sleep(sf::milliseconds(200));
	}

	std::cout << "ChunkManager: World Generation Finalized.\n\n";
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

void ChunkManager::updateLoadingProgress(LoadingScreen* t_loadingScreen, const std::string& t_task, int t_completed, int t_total)
{
	if (!t_loadingScreen)
		return;

	t_loadingScreen->setTask(t_task);
	t_loadingScreen->setProgress(static_cast<float>(t_completed) /
								 static_cast<float>(t_total));
	t_loadingScreen->render();
}

/* - Generation Pass Functions - */


void ChunkManager::buildChunkGrid(LoadingScreen* t_loadingScreen)
{
	updateLoadingProgress(t_loadingScreen, "Generating Chunks...", 0, 1);

	m_chunks.resize(m_chunkCountX);
	for (int x = 0; x < m_chunkCountX; x++)
		for (int y = 0; y < m_chunkCountY; y++)
			m_chunks[x].emplace_back(ChunkPosition{ x, y });

	updateLoadingProgress(t_loadingScreen, "Generating Chunks...", 1, 1);
}

void ChunkManager::assignBiomes(uint32_t t_seed, LoadingScreen* t_loadingScreen)
{
	int l_total = m_chunkCountX * m_chunkCountY;
	int l_completed = 0;

	updateLoadingProgress(t_loadingScreen, "Assigning Biomes...", 0, l_total);

	for (int x = 0; x < m_chunkCountX; x++)
	{
		for (int y = 0; y < m_chunkCountY; y++)
		{
			float l_biomeValue = m_worldGen.getBiomeValue(
				static_cast<int>(x * BIOME_SAMPLE_SCALE),
				static_cast<int>(y * BIOME_SAMPLE_SCALE),
				t_seed
			);
			m_chunks[x][y].setBiomeType(determineBiomeType(l_biomeValue));
			updateLoadingProgress(t_loadingScreen, "Assigning Biomes...", ++l_completed, l_total);
		}
	}
}

void ChunkManager::generateTerrain(uint32_t t_seed, TileMap& t_tileMap, LoadingScreen* t_loadingScreen, const TileVisuals& t_tileVisuals)
{
	int l_total = m_chunkCountX * m_chunkCountY;
	int l_completed = 0;

	updateLoadingProgress(t_loadingScreen, "Generating Terrain...", 0, l_total);

	for (int x = 0; x < m_chunkCountX; x++)
	{
		for (int y = 0; y < m_chunkCountY; y++)
		{
			Chunk& l_chunk = m_chunks[x][y];
			m_worldGen.generateChunk(l_chunk, t_seed, t_tileVisuals);
			updateLoadingProgress(t_loadingScreen, "Generating Terrain...", ++l_completed, l_total);
		}
	}

	// Initial sync so TileMap reflects base terrain before further passes
	syncToTileMap(t_tileMap);
}

void ChunkManager::adjustOceanDepth(LoadingScreen* t_loadingScreen)
{
	int l_total = m_chunkCountX * m_chunkCountY;
	int l_completed = 0;

	updateLoadingProgress(t_loadingScreen, "Adjusting Ocean Depth...", 0, l_total);

	for (int chunkX = 0; chunkX < m_chunkCountX; chunkX++)
	{
		for (int chunkY = 0; chunkY < m_chunkCountY; chunkY++)
		{
			Chunk& l_chunk = m_chunks[chunkX][chunkY];

			if (l_chunk.getBiome() == BiomeType::Ocean)
			{
				for (int localX = 0; localX < Globals::CHUNK_SIZE; localX++)
				{
					for (int localY = 0; localY < Globals::CHUNK_SIZE; localY++)
					{
						Tile& l_tile = l_chunk.getTile(localX, localY);

						if (l_tile.m_type != TileType::Water)
							continue;

						int l_waterNeighbours = 0;

						for (int dx = -1; dx <= 1; dx++)
						{
							for (int dy = -1; dy <= 1; dy++)
							{
								if (dx == 0 && dy == 0) continue;

								int l_worldX = l_tile.m_gridPosition.x + dx;
								int l_worldY = l_tile.m_gridPosition.y + dy;

								if (l_worldX < 0 || l_worldY < 0 ||
									l_worldX >= Globals::WORLD_WIDTH ||
									l_worldY >= Globals::WORLD_HEIGHT)
									continue;

								int l_neighbourChunkX = l_worldX / Globals::CHUNK_SIZE;
								int l_neighbourChunkY = l_worldY / Globals::CHUNK_SIZE;

								if (!isValidChunkPos(l_neighbourChunkX, l_neighbourChunkY))
									continue;

								Tile& l_neighbour = m_chunks[l_neighbourChunkX][l_neighbourChunkY]
									.getTile(l_worldX % Globals::CHUNK_SIZE,
										l_worldY % Globals::CHUNK_SIZE);

								if (l_neighbour.m_type == TileType::Water)
									l_waterNeighbours++;
							}
						}

						l_tile.m_elevation = OCEAN_MAX_ELEVATION - static_cast<int>(
							(l_waterNeighbours / 8.0f) * static_cast<float>(OCEAN_MAX_ELEVATION));
					}
				}
			}

			updateLoadingProgress(t_loadingScreen, "Adjusting Ocean Depth...", ++l_completed, l_total);
		}
	}
}

void ChunkManager::smoothElevation(int t_iterations, LoadingScreen* t_loadingScreen)
{
	updateLoadingProgress(t_loadingScreen, "Smoothing Terrain...", 0, t_iterations);

	for (int i = 0; i < t_iterations; i++)
	{
		smoothElevationBorders(1);
		updateLoadingProgress(t_loadingScreen, "Smoothing Terrain...", i + 1, t_iterations);
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
					/// Also add in some visual artifacts to signify interactability clearly - maybe
					/// not something to add here specifically but a note all the same
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
						l_tile.m_resource.m_resourceID = 5; // Sand
						l_tile.m_resource.m_maxQuantity = (l_biome == BiomeType::Desert) ? 100 : 60;
						l_tile.m_resource.m_currentQuantity = l_tile.m_resource.m_maxQuantity;
						l_tile.m_resource.m_interactionType = InteractionType::Shovel;
						break;

					case TileType::Grass:
						l_tile.m_resource.m_resourceID = 6; // Dirt
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

void ChunkManager::syncToTileMap(TileMap& t_tileMap)
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
					Tile* l_mapTile = t_tileMap.getTileAt(
						l_tile.m_gridPosition.x,
						l_tile.m_gridPosition.y
					);

					// TileMap version of checked data so reflections are accurate
					if (!l_mapTile)
						continue;

					l_mapTile->m_type = l_tile.m_type;
					l_mapTile->m_elevation = l_tile.m_elevation;
					l_mapTile->m_resource = l_tile.m_resource;
					l_mapTile->m_variantIndex = l_tile.m_variantIndex;
					l_mapTile->m_rotationStep = l_tile.m_rotationStep;
				}
			}
		}
	}
}


