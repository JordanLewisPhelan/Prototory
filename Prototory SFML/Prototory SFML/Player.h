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

	sf::RectangleShape m_playerSprite;

	sf::Vector2f getInputDirection() const;
	void constrainToWorldBounds(const TileMap& t_tileMap);
	float clampMagnitude(float t_value, float t_maxMag);

};