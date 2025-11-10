#pragma once
#include "Tile.h"
#include <vector>
#include "Globals.h"


class TileMap
{
public:
	TileMap(int t_width, int t_height, int t_tileSize);

	void render(sf::RenderWindow& t_window);

	// - Accessors -
	Tile* getTileAt(int t_x, int t_y);
	int getWidth() const;
	int getHeight() const;

	// - Adjustors -
	void setTileAt(int t_x, int t_y, TileType t_type);

	// - World & Grid conversions -
	sf::Vector2f gridToWorld(const sf::Vector2i& t_gridPos) const;
	sf::Vector2i worldToGrid(const sf::Vector2f& t_worldPos) const;

private:
	/* - local TileMap usage variables - */
	int m_width { 0 };
	int m_height { 0 };
	int m_tileSize { 0 };

	std::vector<std::vector<Tile>> m_tiles;	// 2D Grid - MENTIONING TO AVOID A REPEAT MISTAKE


	/* - Private Functions - */
	// Helper for determining tile colour based on type
	sf::Color getTileColour(TileType t_type) const;

	// Bound checker
	bool isInBounds(int t_x, int t_y) const;
};
