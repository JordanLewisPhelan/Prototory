#pragma once

#include "Chunk.h"
#include "Globals.h"
#include "TileMap.h"
#include "LoadingScreen.h"
#include "WorldGenerator.h"
#include <stdexcept>	

struct BorderRule {
	BiomeType sourceBiome;
	BiomeType neighbourBiome;
	TileType overrideTile;
};

class ChunkManager
{
public:
	ChunkManager();

	
	void initialize(uint32_t t_seed, TileMap& t_tileMap, LoadingScreen* t_loadingScreen);

	Chunk& getChunk(int t_x, int t_y);

	bool isValidChunkPos(int t_x, int t_y) const;

	int getChunkWidth() { return m_chunkCountX; }
	int getChunkHeight() { return m_chunkCountY; }

	BiomeType determineBiomeType(float t_value) const;

private:
	std::vector<std::vector<Chunk>> m_chunks;
	WorldGenerator m_worldGen;
	int m_chunkCountX;	int m_chunkCountY;
	uint32_t m_seed;

	static constexpr int OCEAN_MAX_ELEVATION = 3;
	static constexpr float BIOME_SAMPLE_SCALE = 127.3f;

	
		
	void updateLoadingProgress(LoadingScreen* t_loadingScreen,
		const std::string& t_task,
		int t_completed,
		int t_total);

		
	
	
	
	void buildChunkGrid(LoadingScreen* t_loadingScreen);

	
	void assignBiomes(uint32_t t_seed, LoadingScreen* t_loadingScreen);

	
	void generateTerrain(uint32_t t_seed, TileMap& t_tileMap, LoadingScreen* t_loadingScreen);

	
	void adjustOceanDepth(LoadingScreen* t_loadingScreen);

	
	void smoothElevation(int t_iterations, LoadingScreen* t_loadingScreen);

	
	
	int getBiomeElevationDelta(BiomeType t_biome) const;

	
	void syncToTileMap(TileMap& t_tileMap);

	
	
	void smoothElevationBorders(int t_iterations);

	
	
	
	
	
	void applyBorderBlending();

	
	
	void assignTileResources();
};