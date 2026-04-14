#pragma once
#include "Tile.h"
#include <vector>
#include "Globals.h"

class TileAccessor;

class TileMap
{
public:
	TileMap(int t_width, int t_height, int t_tileSize);

	void render(sf::RenderWindow& t_window, TileAccessor& t_tileAccessor);

	
	Tile* getTileAt(int t_x, int t_y);
	int getWidth() const;
	int getHeight() const;
	int getElevationAt(int t_x, int t_y) const;
	TileResource& getTileResourceAt(int t_x, int t_y);

	
	void setTileAt(int t_x, int t_y, TileType t_type);
	void setTileAt(int t_x, int t_y, TileType t_type, int t_elevation);

	
	sf::Vector2f gridToWorld(const sf::Vector2i& t_gridPos) const;
	sf::Vector2i worldToGrid(const sf::Vector2f& t_worldPos) const;

	
	bool getDebugElevation() { return m_debugElevation; }
	bool toggleDebugElevation() { return m_debugElevation = !m_debugElevation; }
	void renderDebugElevation(sf::RenderWindow& t_window, const sf::Font& t_font);

private:
	
	int m_width { 0 };
	int m_height { 0 };
	int m_tileSize { 0 };

	std::vector<std::vector<Tile>> m_tiles;	

	
	
	sf::Color getTileColour(TileType t_type) const;

	
	sf::Color applyElevationShade(sf::Color t_base, int t_elevation, int t_minElevation, int t_maxElevation) const;

	
	int getMinElevation(TileType t_type) const;
	int getMaxElevation(TileType t_type) const;

	
	bool isInBounds(int t_x, int t_y) const;

	
	bool m_debugElevation = false;
};
