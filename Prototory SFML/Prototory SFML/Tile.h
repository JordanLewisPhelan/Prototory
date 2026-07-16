#pragma once

#include <optional>
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "MachineComponent.h"


// Simple list of what is available 
enum class TileType
{
	Grass, Trees,
	Stone, IronOre,
	Sand, Water
};

struct TileResource
{
	uint32_t m_resourceID = { 0 };
	int m_currentQuantity = { 0 };
	int m_maxQuantity = { 0 };
	InteractionType m_interactionType = { InteractionType::None };

	bool isHarvestable() const { return m_currentQuantity > 0; }
};

// Simple struct will only be a constructor with some variables
struct Tile {
	TileType m_type;		// The type associated with the tile.
	sf::Vector2i m_gridPosition;	// Vector of where in the TileMap will this be housed
	int m_elevation;			// determining height of a map tile
	int m_variantIndex = 0;		// Sprite Allocated
	int m_rotationStep = 0;
	TileResource m_resource;	// Resource and its attributes associated with this tile
	std::optional<MachineComponent> m_machine;		// What piece of machinery occupies this tile

	// Default constructor for grid initialisation
	Tile()
		: m_type(TileType::Grass)
		, m_gridPosition(0, 0)
		, m_elevation(0)
		, m_resource()
		, m_machine(std::nullopt)
	{}

	Tile(TileType t_type, sf::Vector2i t_gridPos, int t_elevation = 0)
		: m_type(t_type), m_gridPosition(t_gridPos), m_elevation(t_elevation), m_resource(), m_machine(std::nullopt) {}
};