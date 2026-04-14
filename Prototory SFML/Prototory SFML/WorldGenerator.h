#pragma once

#include "TileMap.h"
#include "INoiseGenerator.h"

#include "HashNoiseGenerator.h"
#include "PerlinNoiseGenerator.h"

#include "LoadingScreen.h"
#include "chunk.h"
#include <iostream>
#include <memory>
#include <chrono>	

class WorldGenerator
{
public:
	WorldGenerator();

	
	void generateSeededWorld(TileMap& t_tileMap, uint32_t t_seed);

	
	void generateSeededWorld(TileMap& t_tileMap, uint32_t t_seed, LoadingScreen* t_loadingScreen);

	
	void generateWorld(TileMap& t_tileMap);

	
	void generateChunk(Chunk& t_chunk, uint32_t t_seed);

	
	
	

	float getBiomeValue(int t_x, int t_y, uint32_t t_seed);

	int calculateElevation(float t_value, BiomeType t_biome) const;

	
	uint32_t getCurrentSeed() const { return m_currentSeed; }
	
	
	uint32_t generateSeed();

private:

					
					
					

	PerlinNoiseGenerator m_terrainGen;
	HashNoiseGenerator m_biomeGen;

	uint32_t m_currentSeed;

	
	
	static constexpr uint32_t ELEVATION_SEED_OFFSET = 1000000u;
	

	
	TileType determineTileType(float t_value, BiomeType t_biome) const;
};