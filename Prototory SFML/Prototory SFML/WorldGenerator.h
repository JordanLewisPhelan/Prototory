#pragma once

#include "TileMap.h"
#include "INoiseGenerator.h"
#include <iostream>
#include <memory>
#include <chrono>	//std::chrono


class WorldGenerator
{
public:
	WorldGenerator();

	// Generates a world based on an inputted seed
	void generateSeededWorld(TileMap& t_tileMap, uint32_t t_seed);

	// Generates a world off a random seed
	void generateWorld(TileMap& t_tileMap);

	// Applies or changes used Noise Algorithm
	void setNoiseGenerator(std::unique_ptr<INoiseGenerator> t_generator);

	// Gets the current worlds seed
	uint32_t getCurrentSeed() const { return m_currentSeed; }

private:
	std::unique_ptr<INoiseGenerator> m_noiseGen;
	uint32_t m_currentSeed;

	// Generates random seed for game and returns it
	uint32_t generateSeed();

	// Simply determines what a Tile should be based on the Noise Value
	TileType determineTileType(float t_value) const;
};