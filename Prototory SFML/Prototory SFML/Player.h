#pragma once

#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "Inventory.h"
#include "ResourceRegistry.h" // ToDo: Refactor, this SHOULD NOT be coupled like this but deadlines dont care about structure
#include <cmath>
#include <iostream>


class Player
{
public:
	Player(const sf::Vector2f& t_startPos, const sf::Vector2f& t_size = sf::Vector2f(32.f, 32.f));

	void handleInput(sf::Time t_dt);
	//void update(sf::Time t_dt, const TileMap& t_tileMap);		// ToDo: Restore when removing ResourceRegistry
	void update(sf::Time t_dt, TileMap& t_tileMap, const ResourceRegistry& t_resourceRegistry);
	void render(sf::RenderWindow& t_window);

	sf::Vector2f getWorldPosition() { return m_position; }
	sf::Vector2i getGridPosition(const TileMap& t_tileMap) const;
	void setPosition(const sf::Vector2f& t_setPos) { m_position = t_setPos; }

	Inventory& getInventory() { return m_inventory; }

	sf::Vector2f getFacing() const { return m_playerFacing; }

private:
	Inventory m_inventory;

	sf::Vector2f m_position, m_velocity, m_playerSize;
	sf::Vector2f m_playerFacing{0.1f, 0.1f};

	float m_walkSpeed, m_acceleration, m_deceleration;
	float m_blockTimer, m_harvestTimer{ 0.f };

	/* - Consts - sectioned off due to following - */
	// ToDo: Move into globals or like a playerData struct later for post project work
	// Idea is to expand this into a limitation the player can progress through or 
	// circumvent with structures and pre-effort to make traversal feel earned not cheap
	static constexpr int MAX_ELEVATION_DELTA = 2;
	const float BLOCKED_FLASH_DURATION = 0.2f;
	const float HARVEST_INTERVAL = 0.8f;

	const sf::Color BLOCKED_COLOUR = sf::Color(255, 120, 10);


	sf::RectangleShape m_playerSprite;

	// Refactor into some form of Template later - Usage of different indicators
	sf::CircleShape m_directionIndicator;

	sf::Vector2f getInputDirection() const;
	void constrainToWorldBounds(const TileMap& t_tileMap);
	
	// Prevents player movement up inclines in the world that are too steep
	void checkElevationMovement(const TileMap& t_tileMap, sf::Time t_dt);

	// Refactor when removing Registry is being removed from Player - Really need a layer between this
	// or a communication hub to allow queries to not be so coupled to other classes
	void tryHarvest(TileMap& t_tileMap, const ResourceRegistry& t_resourceRegistry);

	void renderDirectionalIndicator(sf::RenderWindow& t_window);

};