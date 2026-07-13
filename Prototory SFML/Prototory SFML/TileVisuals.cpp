#include "TileVisuals.h"


bool TileVisuals::loadSheet(const std::string t_filePath)
{
	if (!m_texture.loadFromFile(t_filePath))
	{
		std::cout << "[TileVisuals] File Path incorrect.\n";
		return false;
	}

	std::cout << "[TileVisuals] File Path Loaded Successfully.\n";
	m_texture.setSmooth(false); // Added to fix resolution blur when scaling
	return true;
}


// COMPLETELY DEBUG AND TEMPORARY
void TileVisuals::renderDEBUG(sf::RenderWindow& t_window, sf::RectangleShape& t_player)
{
	// Notice - 16 is offset of sprite sheet before sprite work starts, effects a lot, dont forget to account for this when writing actual system
	int cell_size = 80;
	int test = Globals::TILE_SIZE + 16;
	TileVisuals t;
	sf::Vector2f l_pos = t_player.getPosition();


	t.loadSheet("ASSETS\\IMAGES\\GrassTest.png");

	sf::Texture l_testTexture = t.getTexture();
	sf::Sprite l_testSprite(l_testTexture);

	l_testSprite.setTextureRect(sf::IntRect(sf::Vector2(0, 0), sf::Vector2(test, test)));
	l_testSprite.setScale(sf::Vector2f(static_cast<float>(test) / static_cast<float>(cell_size),
		static_cast<float>(test) / static_cast<float>(cell_size)));
	l_testSprite.setPosition(l_pos);

	t_window.draw(l_testSprite);
}
