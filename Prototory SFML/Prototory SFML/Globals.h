#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

struct Globals
{
	static unsigned constexpr SCREEN_WIDTH = 1000.f;
	static unsigned constexpr SCREEN_HEIGHT = 800.f;

	static const int TILE_SIZE = 64;
	static const int WORLD_WIDTH = 128;
	static const int WORLD_HEIGHT = 128;
	static const int CHUNK_SIZE = 16;

	
	static constexpr float HARVEST_REACH = 48.f;
};

enum class InteractionType
{
	None, Chop, Mine, Shovel
};
