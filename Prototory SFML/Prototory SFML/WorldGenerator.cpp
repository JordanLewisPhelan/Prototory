#include "WorldGenerator.h"
#include "HashNoiseGenerator.h"
#include "PerlinNoiseGenerator.h"

WorldGenerator::WorldGenerator() : 
	m_noiseGen(std::make_unique<PerlinNoiseGenerator>()),
	m_currentSeed(0)
{
	std::cout << "WorldGenerator: NoiseGenerator Defaulted to PerlinNoise.\n\n";
}

void WorldGenerator::generateSeededWorld(TileMap& t_tileMap, uint32_t t_seed)
{
    generateSeededWorld(t_tileMap, t_seed, nullptr);
}

void WorldGenerator::generateSeededWorld(TileMap& t_tileMap, uint32_t t_seed, LoadingScreen* t_loadingScreen)
{
    //int l_progress = 0.f;
    //int l_tilesToEval = t_tileMap.getWidth() * t_tileMap.getHeight();
    //int l_tilesCompleted = 0;

    //m_currentSeed = t_seed;
    //std::cout << "WorldGenerator: Generating world with seed: " << m_currentSeed << "\n\n";

    //if (t_loadingScreen)
    //{
    //    t_loadingScreen->setTask("Generating Terrain..");
    //    t_loadingScreen->setProgress(0.0f);
    //    t_loadingScreen->render();
    //}
    /// ^ Currently redundant, logic moved to ChunkManager


    // Iterate through each tile & generate noise
    for (int x = 0; x < t_tileMap.getWidth(); x++)
    {
        for (int y = 0; y < t_tileMap.getHeight(); y++)
        {
            // Get noise for this grid position
            float l_noiseValue = m_noiseGen->generate(x, y, m_currentSeed);

            // Affirm the type of tile it will be
            TileType t_type = determineTileType(l_noiseValue);

            t_tileMap.setTileAt(x, y, t_type);
            
            // Initial implementation, unused but retaining in case want to debug 
     /*       l_tilesCompleted++;


            int l_currentProgress = (l_tilesCompleted * 100) / l_tilesToEval;

            if (l_currentProgress != l_progress && t_loadingScreen)
            {
                l_progress = l_currentProgress;

                float l_percent = static_cast<float>(l_tilesCompleted) / static_cast<float>(l_tilesToEval);

                t_loadingScreen->setProgress(l_percent);
                t_loadingScreen->render();

            }*/
        }
    }

    // TODO: REMOVE LATER OR COMMENT OUT FOR WHEN ADDING MORE PROCESSES ARE TO BE ADDED
    if (t_loadingScreen)
    {
        t_loadingScreen->setTask("Generation Completed");
        t_loadingScreen->setProgress(1.0f);
        t_loadingScreen->render();

        // Forced sleep to ensure it updates for debugging
        sf::sleep(sf::milliseconds(200));
    }

    std::cout << "WorldGenerator: Map developed with noise. \n\n";
}

void WorldGenerator::generateWorld(TileMap& t_tileMap)
{
	uint32_t l_seed = generateSeed();

	generateSeededWorld(t_tileMap, l_seed);
}


void WorldGenerator::generateChunk(Chunk& t_chunk, uint32_t t_seed)
{
    ChunkPosition l_chunkPos = t_chunk.getPosition();

    for (int x = 0; x < Globals::CHUNK_SIZE; x++)
    {
        for (int y = 0; y < Globals::CHUNK_SIZE; y++)
        {
            // Convert Chunk Space into area for Noise to be applied
            int l_worldX = (l_chunkPos.x * Globals::CHUNK_SIZE) + x;
            int l_worldY = (l_chunkPos.y * Globals::CHUNK_SIZE) + y;

            float l_noiseVal = m_noiseGen->generate(l_worldX, l_worldY, t_seed);
            TileType l_tileType = determineTileType(l_noiseVal);

            t_chunk.getTile(x, y).m_type = l_tileType;
            t_chunk.getTile(x, y).m_gridPosition = sf::Vector2i(l_worldX, l_worldY);

        }
    }
}


void WorldGenerator::setNoiseGenerator(std::unique_ptr<INoiseGenerator> t_generator)
{
	m_noiseGen = std::move(t_generator);
	std::cout << "WorldGenerator: Generation type has changed.\n\n";
}

uint32_t WorldGenerator::generateSeed()
{
    // Uses std::chrono high resolution clock to get time for seed
    auto l_now = std::chrono::high_resolution_clock::now();
    auto l_duration = l_now.time_since_epoch();

	return static_cast<uint32_t>(l_duration.count());
}

TileType WorldGenerator::determineTileType(float t_value) const
{
    // Map noise value [0.0, 1.0] to tile types
    // These thresholds determine tile distribution

    if (t_value < 0.2f)          // e.g 20% of tiles
        return TileType::Water;
    else if (t_value < 0.5f)     // e.g 30% of tiles
        return TileType::Grass;
    else if (t_value < 0.75f)    
        return TileType::Forest;
    else if (t_value < 0.95f)    
        return TileType::Stone;
    else                             
        return TileType::IronOre;
}
