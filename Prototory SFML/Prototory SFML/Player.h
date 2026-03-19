#pragma once

#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include <cmath>
#include <iostream>


class Player
{
public:
	Player(const sf::Vector2f& t_startPos, const sf::Vector2f& t_size = sf::Vector2f(32.f, 32.f));

	void handleInput(sf::Time t_dt);
	void update(sf::Time t_dt, const TileMap& t_tileMap);
	void render(sf::RenderWindow& t_window);

	sf::Vector2f getWorldPosition() { return m_position; }
	sf::Vector2i getGridPosition(const TileMap& t_tileMap) const;
	void setPosition(const sf::Vector2f& t_setPos) { m_position = t_setPos; }

private:
	sf::Vector2f m_position, m_velocity;
	sf::Vector2f m_playerSize;
	float m_walkSpeed;
	float m_acceleration;
	float m_deceleration;
	float m_blockTimer = 0.f;	

	/* - Consts - sectioned off due to following - */
	// ToDo: Move into globals or like a playerData struct later for post project work
	// Idea is to expand this into a limitation the player can progress through or 
	// circumvent with structures and pre-effort to make traversal feel earned not cheap
	static constexpr int MAX_ELEVATION_DELTA = 2;

	const float BLOCKED_FLASH_DURATION = 0.2f;
	const sf::Color BLOCKED_COLOUR = sf::Color(255, 120, 10);


	sf::RectangleShape m_playerSprite;

	sf::Vector2f getInputDirection() const;
	void constrainToWorldBounds(const TileMap& t_tileMap);
	
	// Prevents player movement up inclines in the world that are too steep
	void checkElevationMovement(const TileMap& t_tileMap, sf::Time t_dt);

};