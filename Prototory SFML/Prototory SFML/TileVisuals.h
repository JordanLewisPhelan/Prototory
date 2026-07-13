#pragma once
#include "Globals.h"
#include <vector>
#include <cstdint>
#include "Tile.h"


class TileVisuals
{
public:
	bool loadSheet(const std::string t_filePath);

	const sf::Texture& getTexture() const { return m_texture; }

	// To REMOVE
	void renderDEBUG(sf::RenderWindow& t_window, sf::RectangleShape& t_player);

private:

	sf::Texture m_texture;
};