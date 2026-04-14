#pragma once

#include <optional>
#include <SFML/Graphics.hpp>
#include "MachineRegistry.h"
#include "TileAccessor.h"
#include "MachineDefinition.h"
#include "MachineSystem.h"

enum class PlacementIntent
{
	None, Place, Remove
};

struct PlacementRequest
{
	PlacementIntent m_intent{ PlacementIntent::None };
	uint32_t m_definitionID{ 0 };
	sf::Vector2i m_gridPos{ 0,0 };
	sf::Vector2f m_worldOffset{ 0.f, 0.f };
	sf::Vector2f m_facingDirection{ 0.f, 0.f };
	bool m_valid{ false };
};

class PlacementController
{
public:
	PlacementController(const MachineRegistry& t_registry, TileAccessor& t_tileAccessor, const sf::Font& t_font);

	
	PlacementRequest update(const sf::Vector2f& t_playerPos,
							const sf::Vector2f& t_playerFacing,
							const MachineSystem& t_machineSystem);

	
	
	
	
	void handleEvent(const sf::Event& t_event);

	
	void render(sf::RenderWindow& t_window);

	bool hasSelection() const;
	uint32_t getSelectedId() const;

private:
	const MachineRegistry& m_machineRegistry;
	TileAccessor& m_tileAccessor;

	std::optional<uint32_t> m_selectedDefinitionID;
	std::optional<sf::Vector2i> m_targetTile;
	bool m_placementValid{ false };
	bool m_confirmPlace{ false };
	bool m_confirmRemove{ false };

	sf::RectangleShape m_highlightShape;

	sf::Text m_selectionLabel;

	
	sf::Vector2i determineHighlightTile(const sf::Vector2f& t_playerPos,
										const sf::Vector2f& t_playerFacing) const;

	
	sf::Vector2f determineHighlightOffset(const sf::Vector2f& t_playerFacing) const;

	
	bool checkValidity(sf::Vector2i t_gridPos,
					  const MachineDefinition& t_def,
					  const MachineSystem& t_machineSystem) const;
};