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


	// Utilizes seed set by Game to generate the Chunk Grid
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

	// Returns a simple int value set of what is the allowance for Chunks
	// to allow their heights to be to prevent steep inclines-declines
	int getBiomeElevationDelta(BiomeType t_biome) const;

	// Iterates over the Elevations <iterations> number of times to
	// smooth out and alleviate the steepness issues 
	void smoothElevationBorders(int t_iterations);

	// Uses BorderRule struct to determine how to transfer from one biome
	// to another biome more cohesively.
	/// ToDo: Refactor after project deadline to be more cohesive overall
	/// and apply this concept and a larger class/struct to other shameful
	/// code slots that could use it, e.g. BiomeElevationDelta rules
	void applyBorderBlending();

	// Does a pass based on Biome to assign Resource Spawns
	// Will be updated later as a pass to enforce a resource average per chunk
	void assignTileResources();

};