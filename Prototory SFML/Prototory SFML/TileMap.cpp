#include "TileMap.h"



/// Notice: Due to 2D vector storage, going by width and then by height with reserve is best practice
// Constructor
TileMap::TileMap(int t_width, int t_height, int t_tileSize)
	: m_width(t_width), m_height(t_width), m_tileSize(t_tileSize)
{
	// Creates outer vector - one vector per row (x-axis)
	m_tiles.resize(m_width);

	for (int x = 0; x < m_width; ++x)
	{
		// reserve space vertically as we move to a new row
		m_tiles[x].reserve(m_height);

		for (int y = 0; y < m_height; ++y)
		{
			// Adds a tile to this row with its grid position
			m_tiles[x].emplace_back(TileType::Grass, sf::Vector2i(x, y));
		}
	}
		
	std::cout << "TileMap[Debug]: TileMap created with: " << m_width << "x" << m_height
		<< " (" << (m_width * m_height) << " tiles generated) \n";

}


void TileMap::render(sf::RenderWindow& t_window)
{
	// This will determine what we can see on screen
	sf::View l_view = t_window.getView();			// Effectively 2D camera 
	sf::Vector2f l_viewSize = l_view.getSize();		// Gets dimensions of window
	sf::Vector2f l_viewCenter = l_view.getCenter(); // Gets the views center, we can use that to figure out easily what will be rendered just next

	// Calculating what tiles are to be rendered based on screen space and world placement - with a bit of a margin of error - avoids tiles disappearing
	int minX = std::max(0, static_cast<int>((l_viewCenter.x - l_viewSize.x / 2.f) / m_tileSize) - 1);
	int maxX = std::min(m_width, static_cast<int>((l_viewCenter.x + l_viewSize.x / 2.f) / m_tileSize) + 1);
	int minY = std::max(0, static_cast<int>((l_viewCenter.y - l_viewSize.y / 2.f) / m_tileSize) - 1);
	int maxY = std::min(m_height, static_cast<int>((l_viewCenter.y + l_viewSize.y / 2.f) / m_tileSize) + 1);


	// Create a reusable drawable rectangle.
	sf::RectangleShape l_tileRectangle(sf::Vector2f(m_tileSize, m_tileSize));

	// We only draw visible tiles
	for (int x = minX; x < maxX; ++x)
	{
		for (int y = minY; y < maxY; ++y)
		{
			const Tile& l_tile = m_tiles[x][y];

			sf::Color l_baseColour = getTileColour(l_tile.m_type);
			sf::Color l_finalColour = applyElevationShade(l_baseColour, l_tile.m_elevation, 
															getMinElevation(l_tile.m_type),
															getMaxElevation(l_tile.m_type));

			// ToDo: Refactor to a more scalable model later should it be required.
			l_tileRectangle.setPosition(sf::Vector2f(x * m_tileSize, y * m_tileSize));
			l_tileRectangle.setFillColor(l_finalColour);
			l_tileRectangle.setOutlineThickness(1.f);
			l_tileRectangle.setOutlineColor(sf::Color::Black);

			t_window.draw(l_tileRectangle);

		}
	}
}

/* - Public Getters - */
// Returns a pointer to tile at grid position, or nullptr if out of bounds
Tile* TileMap::getTileAt(int t_x, int t_y)
{
	if (!isInBounds(t_x, t_y))
	{
		return nullptr;
	}

	return &m_tiles[t_x][t_y];
}

int TileMap::getWidth() const
{
	return m_width;
}

int TileMap::getHeight() const
{
	return m_height;
}


/* - Public Setter - */
// Sets a tile at a specified Tile[x][y] index to a specified tileType
void TileMap::setTileAt(int t_x, int t_y, TileType t_type)
{
	if (isInBounds(t_x, t_y))
	{
		m_tiles[t_x][t_y].m_type = t_type;
	}
}

void TileMap::setTileAt(int t_x, int t_y, TileType t_type, int t_elevation)
{
	if (isInBounds(t_x, t_y))
	{
		m_tiles[t_x][t_y].m_type = t_type;
		m_tiles[t_x][t_y].m_elevation = t_elevation;
	}
}


/* - Grid & World converters - */
// Takes in a gridPosition of TileMap[x][y] and returns a world position matching that
sf::Vector2f TileMap::gridToWorld(const sf::Vector2i& t_gridPos) const
{
	// We will return in the center of the tile 
	return sf::Vector2f(
		t_gridPos.x * m_tileSize + m_tileSize / 2.f,
		t_gridPos.y * m_tileSize + m_tileSize / 2.f
	);
}

// Takes in a world position and returns a TileMap[x][y] matching that
sf::Vector2i TileMap::worldToGrid(const sf::Vector2f& t_worldPos) const
{
	return sf::Vector2i(
		static_cast<int>(t_worldPos.x / m_tileSize),
		static_cast<int>(t_worldPos.y / m_tileSize)
	);
}


/* - Private Functions - */
// determines what colour a Tile should be based on the type
sf::Color TileMap::getTileColour(TileType t_type) const
{
	switch (t_type)
	{
	case TileType::Grass:
		return sf::Color(34, 139, 34);      // Green
	case TileType::Trees:
		return sf::Color(111, 78, 55);        // Brown
	case TileType::Stone:
		return sf::Color(128, 128, 128);    // Gray
	case TileType::IronOre:
		return sf::Color(217, 144, 88);      // Faded Orange
	case TileType::Sand:
		return sf::Color(239, 255, 40);		// Yellow
	case TileType::Water:
		return sf::Color(30, 144, 255);     // Dodger blue
	default:
		return sf::Color::Magenta;          // Error - shouldn't happen
	}
}

sf::Color TileMap::applyElevationShade(sf::Color t_base, int t_elevation, int t_minElevation, int t_maxElevation) const
{

	float l_dark = 0.75f;
	float l_light = 1.3f;

	// Normalizing the value based on Biome ranges
	float l_t = static_cast<float>(t_elevation - t_minElevation) /
				static_cast<float>(t_maxElevation - t_minElevation);

	// Scalar value to lighten or deepen the shade of the colour of the tile
	float l_scalar = l_dark + (l_t * (l_dark - l_light));

	return sf::Color(
		static_cast<uint8_t>(std::clamp(static_cast<float>(t_base.r) * l_scalar, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(static_cast<float>(t_base.g) * l_scalar, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(static_cast<float>(t_base.b) * l_scalar, 0.f, 255.f)),
		255
	);
}

int TileMap::getMinElevation(TileType t_type) const
{
	// ToDo: Refactor to use constant values - for now hard coded is fine
	switch (t_type)
	{
	case TileType::Water:   return 0;
	case TileType::Grass:   return 4;
	case TileType::Trees:   return 5;
	case TileType::Sand:    return 7;
	case TileType::Stone:   return 7;
	case TileType::IronOre: return 7;
	default:                return 0;
	}
}

int TileMap::getMaxElevation(TileType t_type) const
{
	// ToDo: Refactor to use constant values - for now hard coded is fine
	switch (t_type)
	{
	case TileType::Water:   return 3;
	case TileType::Grass:   return 13;
	case TileType::Trees:   return 10;
	case TileType::Sand:    return 11;
	case TileType::Stone:   return 25;
	case TileType::IronOre: return 25;
	default:                return 10;
	}
}

// checks to see if the Tile queried is inside the TileMaps bounds
bool TileMap::isInBounds(int t_x, int t_y) const
{
	return t_x >= 0 && t_x < m_width && t_y >= 0 && t_y < m_height;
}
