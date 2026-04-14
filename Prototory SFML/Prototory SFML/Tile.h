#pragma once

#include <optional>
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "MachineComponent.h"

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

struct Tile {
	TileType m_type;		
	sf::Vector2i m_gridPosition;	
	int m_elevation;			
	TileResource m_resource;	
	std::optional<MachineComponent> m_machine;		

	
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