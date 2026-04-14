#include "TileMap.h"
#include "TileAccessor.h"

TileMap::TileMap(int t_width, int t_height, int t_tileSize)
	: m_width(t_width), m_height(t_width), m_tileSize(t_tileSize)
{
	
	m_tiles.resize(m_width);

	for (int x = 0; x < m_width; ++x)
	{
		
		m_tiles[x].reserve(m_height);

		for (int y = 0; y < m_height; ++y)
		{
			
			m_tiles[x].emplace_back(TileType::Grass, sf::Vector2i(x, y));
		}
	}
		
	std::cout << "TileMap[Debug]: TileMap created with: " << m_width << "x" << m_height
		<< " (" << (m_width * m_height) << " tiles generated) \n";

}

void TileMap::render(sf::RenderWindow& t_window, TileAccessor& t_tileAccessor)
{
	
	sf::View l_view = t_window.getView();			
	sf::Vector2f l_viewSize = l_view.getSize();		
	sf::Vector2f l_viewCenter = l_view.getCenter(); 

	
	int minX = std::max(0, static_cast<int>((l_viewCenter.x - l_viewSize.x / 2.f) / m_tileSize) - 1);
	int maxX = std::min(m_width, static_cast<int>((l_viewCenter.x + l_viewSize.x / 2.f) / m_tileSize) + 1);
	int minY = std::max(0, static_cast<int>((l_viewCenter.y - l_viewSize.y / 2.f) / m_tileSize) - 1);
	int maxY = std::min(m_height, static_cast<int>((l_viewCenter.y + l_viewSize.y / 2.f) / m_tileSize) + 1);

	
	sf::RectangleShape l_tileRectangle(sf::Vector2f(m_tileSize, m_tileSize));

	
	for (int x = minX; x < maxX; ++x)
	{
		for (int y = minY; y < maxY; ++y)
		{
			const Tile& l_tile = m_tiles[x][y];

			sf::Color l_baseColour = getTileColour(l_tile.m_type);
			sf::Color l_finalColour = applyElevationShade(l_baseColour, l_tile.m_elevation,
				getMinElevation(l_tile.m_type),
				getMaxElevation(l_tile.m_type));

			
			l_tileRectangle.setPosition(sf::Vector2f(x * m_tileSize, y * m_tileSize));
			l_tileRectangle.setFillColor(l_finalColour);
			l_tileRectangle.setOutlineThickness(1.f);
			l_tileRectangle.setOutlineColor(sf::Color::Black);

			t_window.draw(l_tileRectangle);

			
			Tile* l_liveTile = t_tileAccessor.get(sf::Vector2i(x, y));
			if (l_liveTile && l_liveTile->m_machine.has_value())
			{
				const MachineVisual& l_visual = l_liveTile->m_machine->getVisual();
				sf::RectangleShape l_machineShape;

				l_machineShape.setSize(l_visual.m_size);
				l_machineShape.setFillColor(l_visual.m_idle
					? sf::Color(120, 120, 120)
					: l_visual.m_colour);
				l_machineShape.setOutlineColor(sf::Color::Black);
				l_machineShape.setOutlineThickness(1.f);

				
				sf::Vector2f l_centre(
					x * m_tileSize + (m_tileSize - l_visual.m_size.x) / 2.f,
					y * m_tileSize + (m_tileSize - l_visual.m_size.y) / 2.f
				);

				l_machineShape.setPosition(l_centre + l_visual.m_offsetPos);
				t_window.draw(l_machineShape);
			}
		}
	}
}

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

int TileMap::getElevationAt(int t_x, int t_y) const
{
	if (!isInBounds(t_x, t_y))
	{
		return 0;
	}

	return m_tiles[t_x][t_y].m_elevation;
}

TileResource& TileMap::getTileResourceAt(int t_x, int t_y)
{
	return m_tiles[t_y][t_x].m_resource;
}

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

sf::Vector2f TileMap::gridToWorld(const sf::Vector2i& t_gridPos) const
{
	
	return sf::Vector2f(
		t_gridPos.x * m_tileSize + m_tileSize / 2.f,
		t_gridPos.y * m_tileSize + m_tileSize / 2.f
	);
}

sf::Vector2i TileMap::worldToGrid(const sf::Vector2f& t_worldPos) const
{
	return sf::Vector2i(
		static_cast<int>(t_worldPos.x / m_tileSize),
		static_cast<int>(t_worldPos.y / m_tileSize)
	);
}

sf::Color TileMap::getTileColour(TileType t_type) const
{
	switch (t_type)
	{
	case TileType::Grass:
		return sf::Color(34, 139, 34);      
	case TileType::Trees:
		return sf::Color(111, 78, 55);        
	case TileType::Stone:
		return sf::Color(128, 128, 128);    
	case TileType::IronOre:
		return sf::Color(217, 144, 88);      
	case TileType::Sand:
		return sf::Color(239, 255, 40);		
	case TileType::Water:
		return sf::Color(30, 144, 255);     
	default:
		return sf::Color::Magenta;          
	}
}

sf::Color TileMap::applyElevationShade(sf::Color t_base, int t_elevation, int t_minElevation, int t_maxElevation) const
{

	float l_dark = 0.85f;
	float l_light = 1.5f;

	
	float l_t = static_cast<float>(t_elevation - t_minElevation) /
				static_cast<float>(t_maxElevation - t_minElevation);

	
	float l_scalar = l_light + (l_t * (l_light - l_dark));

	return sf::Color(
		static_cast<uint8_t>(std::clamp(static_cast<float>(t_base.r) * l_scalar, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(static_cast<float>(t_base.g) * l_scalar, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(static_cast<float>(t_base.b) * l_scalar, 0.f, 255.f)),
		255
	);
}

int TileMap::getMinElevation(TileType t_type) const
{
	
	switch (t_type)
	{
	case TileType::Water:   return 0;
	case TileType::Grass:   return 4;
	case TileType::Trees:   return 6;
	case TileType::Sand:    return 4;
	case TileType::Stone:   return 7;
	case TileType::IronOre: return 7;
	default:                return 0;
	}
}

int TileMap::getMaxElevation(TileType t_type) const
{
	
	switch (t_type)
	{
	case TileType::Water:   return 3;
	case TileType::Grass:   return 13;
	case TileType::Trees:   return 14;
	case TileType::Sand:    return 8;
	case TileType::Stone:   return 25;
	case TileType::IronOre: return 25;
	default:                return 10;
	}
}

bool TileMap::isInBounds(int t_x, int t_y) const
{
	return t_x >= 0 && t_x < m_width && t_y >= 0 && t_y < m_height;
}

void TileMap::renderDebugElevation(sf::RenderWindow& t_window, const sf::Font& t_font)
{
	sf::View l_view = t_window.getView();
	sf::Vector2f l_viewSize = l_view.getSize();
	sf::Vector2f l_viewCenter = l_view.getCenter();

	int minX = std::max(0, static_cast<int>((l_viewCenter.x - l_viewSize.x / 2.f) / m_tileSize) - 1);
	int maxX = std::min(m_width, static_cast<int>((l_viewCenter.x + l_viewSize.x / 2.f) / m_tileSize) + 1);
	int minY = std::max(0, static_cast<int>((l_viewCenter.y - l_viewSize.y / 2.f) / m_tileSize) - 1);
	int maxY = std::min(m_height, static_cast<int>((l_viewCenter.y + l_viewSize.y / 2.f) / m_tileSize) + 1);

	sf::Text l_text(t_font);
	l_text.setCharacterSize(10);
	l_text.setFillColor(sf::Color::White);
	l_text.setOutlineColor(sf::Color::Black);
	l_text.setOutlineThickness(1.f);

	for (int x = minX; x < maxX; ++x)
	{
		for (int y = minY; y < maxY; ++y)
		{
			const Tile& l_tile = m_tiles[x][y];
			l_text.setString(std::to_string(l_tile.m_elevation));
			l_text.setPosition(sf::Vector2f(
				x * m_tileSize + 2.f,
				y * m_tileSize + 2.f
			));
			t_window.draw(l_text);
		}
	}
}
