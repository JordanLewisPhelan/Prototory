#pragma once

#include "TileMap.h"
#include "INoiseGenerator.h"
// Remove when re-adding Interfaced Noise
#include "HashNoiseGenerator.h"
#include "PerlinNoiseGenerator.h"
//
#include "LoadingScreen.h"
#include "chunk.h"
#include <iostream>
#include <memory>
#include <chrono>	//std::chrono


class WorldGenerator
{
public:
	WorldGenerator();

	// Generates a world based on an inputted seed
	void generateSeededWorld(TileMap& t_tileMap, uint32_t t_seed);

	// Generates a world based on an inputted seed and transitions to the loading Screen Modal(Proof of concept attempt)
	void generateSeededWorld(TileMap& t_tileMap, uint32_t t_seed, LoadingScreen* t_loadingScreen);

	// Generates a world off a random seed
	void generateWorld(TileMap& t_tileMap);

	// Generates a chunk - sequential
	void generateChunk(Chunk& t_chunk, uint32_t t_seed);

	// Applies or changes used Noise Algorithm
	//void setNoiseGenerator(std::unique_ptr<INoiseGenerator> t_generator);
	//void setBiomeNoiseGeneratro(std::unique_ptr<INoiseGenerator> t_generator);

	float getBiomeValue(int t_x, int t_y, uint32_t t_seed);

	int calculateElevation(float t_value, BiomeType t_biome) const;

	// Gets the current worlds seed
	uint32_t getCurrentSeed() const { return m_currentSeed; }
	
	// Generates random seed for game and returns it
	uint32_t generateSeed();

private:
/// Not useless but not currently used.		
					//std::unique_ptr<INoiseGenerator> m_noiseGen;
					///* Unideal to make 2 Interface Variables but idea to swap at runtime 
					//   is short-sighted Better this way for flow control anyway
					//*/
					//std::unique_ptr<INoiseGenerator> m_biomeNoiseGen;	

	PerlinNoiseGenerator m_terrainGen;
	HashNoiseGenerator m_biomeGen;

	uint32_t m_currentSeed;

	// Elevation is a second terrain noise sample with an offset seed. Hence Large arbitrary number
	// If elevation ever needs its own noise behaviour (e.g. FBM), a dedicated generator can be added here.
	static constexpr uint32_t ELEVATION_SEED_OFFSET = 1000000u;
	

	// Simply determines what a Tile should be based on the Noise Value
	TileType determineTileType(float t_value, BiomeType t_biome) const;
};