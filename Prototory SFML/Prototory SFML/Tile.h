#pragma once
#include "Tile.h"
#include <SFML/Graphics.hpp>

// Simple list of what is available 
enum class TileType
{
	Grass,
	Forest,
	Stone,
	IronOre,
	Water
};

// Simple struct will only be a constructor with some variables
struct Tile {
	TileType m_type;		// The type associated with the tile.
	sf::Vector2i m_gridPosition;	// Vector of where in the TileMap will this be housed
	int m_elevation;			// Unused right now, goal is to try use this later for determining height of a map tile ( 0 is for the ground )


	Tile(TileType t_type, sf::Vector2i t_gridPos, int t_elevation = 0)
		: m_type(t_type), m_gridPosition(t_gridPos), m_elevation(t_elevation) {}
};