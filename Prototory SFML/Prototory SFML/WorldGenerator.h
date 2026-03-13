#pragma once

#include "TileMap.h"
#include "INoiseGenerator.h"
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
	void setNoiseGenerator(std::unique_ptr<INoiseGenerator> t_generator);

	// Gets the current worlds seed
	uint32_t getCurrentSeed() const { return m_currentSeed; }
	
	// Generates random seed for game and returns it
	uint32_t generateSeed();

private:
	std::unique_ptr<INoiseGenerator> m_noiseGen;
	uint32_t m_currentSeed;

	

	// Simply determines what a Tile should be based on the Noise Value
	TileType determineTileType(float t_value) const;
};