#include "TileVisuals.h"


bool TileVisuals::loadTexture(const std::string t_filePath)
{
	if (m_textures.find(t_filePath) != m_textures.end())
	{
		std::cout << "[TileVisuals] File Path incorrect.\n";
		return true;
	}

	sf::Texture l_texture;

	if (!l_texture.loadFromFile(t_filePath))
	{
		std::cout << "[TileVisuals] File Path incorrect.\n";
		return false;
	}

	std::cout << "[TileVisuals] File Path Loaded Successfully.\n";
	l_texture.setSmooth(false); // Added to fix resolution blur when scaling
	m_textures[t_filePath] = std::move(l_texture);
	return true;
}

const sf::Texture* TileVisuals::getTexture(const std::string& t_filePath) const
{
	auto l_it = m_textures.find(t_filePath);

	if (l_it == m_textures.end())
	{ 
		std::cout << "[TileVisuals] Requested texture not loaded: " << t_filePath << "\n";
		return nullptr;
	}

	return &l_it->second;
}

const sf::Texture* TileVisuals::getTextureForType(TileType t_type) const
{
	auto l_pathIt = m_typeToTexturePaths.find(static_cast<int>(t_type));

	if (l_pathIt == m_typeToTexturePaths.end())
	{
		std::cout << "[TileVisuals] No texture path registered for this TileType.\n";
		return nullptr;
	}

	return getTexture(l_pathIt->second);
}

void TileVisuals::registerDefaults()
{
	const std::string l_grassPath = "ASSETS\\IMAGES\\GrassTest.png";

	// Links path to TileType
	if (loadTexture(l_grassPath))
	{
		m_typeToTexturePaths[static_cast<int>(TileType::Grass)] = l_grassPath;

		// Note and reminder of Layouts
		//					rows , columns , Sprite Size , Space between Cells , The Offset to reach the sprite
		GridLayout l_grassLayout{ 3, 3, sf::Vector2i(64,64),  sf::Vector2i(16,16), sf::Vector2i(16,16) };

		for (const sf::IntRect& l_rect : sliceIntoRects(l_grassLayout))
		{
			m_variants[static_cast<int>(TileType::Grass)].push_back(TileVariant{ l_rect });
		}
	}


	const std::string l_stonePath = "ASSETS\\IMAGES\\RockTest.png";

	if (loadTexture(l_stonePath))
	{
		m_typeToTexturePaths[static_cast<int>(TileType::Stone)] = l_stonePath;

		GridLayout l_stoneLayout{ 5, 5, sf::Vector2i(32, 32), sf::Vector2i(16, 16), sf::Vector2i(16, 16) };

		for (const sf::IntRect& l_rect : sliceIntoRects(l_stoneLayout))
		{
			m_variants[static_cast<int>(TileType::Stone)].push_back(TileVariant{ l_rect });
		}
	}
}

sf::IntRect TileVisuals::getVariantRect(TileType t_type, int t_variantIndex) const
{
	auto l_it = m_variants.find(static_cast<int>(t_type));

	if (l_it == m_variants.end() || l_it->second.empty())
	{
		std::cout << "[TileVisuals] No variants registered for this TileType. \n";
		return sf::IntRect();
	}

	int l_index = t_variantIndex % static_cast<int>(l_it->second.size());

	return l_it->second[l_index].m_spriteRect;
}




std::vector<sf::IntRect> sliceIntoRects(const GridLayout& t_layout)
{
	std::vector<sf::IntRect> l_rects;

	if (t_layout.m_rows <= 0 || t_layout.m_cols <= 0)
	{
		std::cout << "[GridLayout] Layout called has No Rows/Columns. ERROR. \n";
		return l_rects;
	}

	for (int row = 0; row < t_layout.m_rows; ++row)
	{
		for (int col = 0; col < t_layout.m_cols; ++col)
		{
			int l_x = t_layout.m_offset.x + (col * (t_layout.m_cellSize.x + t_layout.m_spacing.x));
			int l_y = t_layout.m_offset.y + (row * (t_layout.m_cellSize.y + t_layout.m_spacing.y));

			l_rects.push_back(sf::IntRect(sf::Vector2i(l_x, l_y), t_layout.m_cellSize));
		}
	}

	return l_rects;
}
