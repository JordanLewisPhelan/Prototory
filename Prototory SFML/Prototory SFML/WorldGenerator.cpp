#include "WorldGenerator.h"

WorldGenerator::WorldGenerator() : 
	m_currentSeed(0)
{
	std::cout << "WorldGenerator: NoiseGenerator Defaulted to PerlinNoise.\n\n";
    std::cout << "WorldGenerator: BiomeNoiseGenerator Defaulted to HashNoise.\n\n";
}

void WorldGenerator::generateSeededWorld(TileMap& t_tileMap, uint32_t t_seed)
{
    generateSeededWorld(t_tileMap, t_seed, nullptr);
}

void WorldGenerator::generateSeededWorld(TileMap& t_tileMap, uint32_t t_seed, LoadingScreen* t_loadingScreen)
{
    
    for (int x = 0; x < t_tileMap.getWidth(); x++)
    {
        for (int y = 0; y < t_tileMap.getHeight(); y++)
        {
            
            float l_noiseValue = m_terrainGen.generate(x, y, m_currentSeed);

            
            TileType t_type = determineTileType(l_noiseValue, BiomeType::Plains);

            t_tileMap.setTileAt(x, y, t_type);
        }
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
    BiomeType l_biome = t_chunk.getBiome();

    for (int x = 0; x < Globals::CHUNK_SIZE; x++)
    {
        for (int y = 0; y < Globals::CHUNK_SIZE; y++)
        {
            
            int l_worldX = (l_chunkPos.x * Globals::CHUNK_SIZE) + x;
            int l_worldY = (l_chunkPos.y * Globals::CHUNK_SIZE) + y;

            float l_elevationVal = m_terrainGen.generate(l_worldX, l_worldY, t_seed + ELEVATION_SEED_OFFSET);
            
            float l_terrainVal = m_terrainGen.generate(l_worldX, l_worldY, t_seed);

            int l_elevation = calculateElevation(l_elevationVal, l_biome);

            TileType l_tileType = determineTileType(l_terrainVal, l_biome);

            Tile& l_tile = t_chunk.getTile(x, y);
            l_tile.m_type = l_tileType;
            l_tile.m_gridPosition = sf::Vector2i(l_worldX, l_worldY);
            l_tile.m_elevation = l_elevation;

        }
    }
}

int WorldGenerator::calculateElevation(float t_noiseVal, BiomeType t_biome) const
{
    
    int l_min = 0;
    int l_max = 0;

    switch (t_biome)
    {
    case BiomeType::Ocean:      l_min = 0;  l_max = 3;  break;
    case BiomeType::Plains:     l_min = 4;  l_max = 13; break;
    case BiomeType::Forest:     l_min = 5;  l_max = 12; break;
    case BiomeType::Mountains:  l_min = 14; l_max = 25; break;
    case BiomeType::Desert:     l_min = 7;  l_max = 11; break;
    default:                    l_min = 4;  l_max = 8;  break;
    }

    return l_min + static_cast<int>(t_noiseVal * static_cast<float>(l_max - l_min));
}

float WorldGenerator::getBiomeValue(int t_x, int t_y, uint32_t t_seed)
{
    return m_biomeGen.generate(t_x, t_y, t_seed);
}

uint32_t WorldGenerator::generateSeed()
{
    
    auto l_now = std::chrono::high_resolution_clock::now();
    auto l_duration = l_now.time_since_epoch();

	return static_cast<uint32_t>(l_duration.count());
}

TileType WorldGenerator::determineTileType(float t_value, BiomeType t_biome) const
{
    switch (t_biome)
    {
    case BiomeType::Ocean:
        if (t_value < 0.85f)        return TileType::Water;
        else                        return TileType::Sand;

    case BiomeType::Plains:
        if (t_value < 0.15f)        return TileType::Water;
        else if (t_value < 0.80f)   return TileType::Grass;
        else if (t_value < 0.93f)   return TileType::Stone;
        else                        return TileType::IronOre;

    case BiomeType::Forest:
        if (t_value < 0.20f)        return TileType::Grass;
        else if (t_value < 0.82f)   return TileType::Trees;
        else                        return TileType::Stone;

    case BiomeType::Mountains:
        if (t_value < 0.20f)        return TileType::Grass;
        else if (t_value < 0.60f)   return TileType::Stone;
        else if (t_value < 0.85f)   return TileType::IronOre;
        else                        return TileType::Stone;

    case BiomeType::Desert:
        if (t_value < 0.75f)        return TileType::Sand;
        else if (t_value < 0.92f)   return TileType::Stone;
        else                        return TileType::IronOre;

    default:
        return TileType::Grass;
    }
}

