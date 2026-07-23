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


void TileVisuals::registerTileType(TileType t_type, const std::string& t_filePath, const GridLayout& t_layout)
{
	if (!loadTexture(t_filePath))
	{
		std::cout << "[TileVisuals] Requested file path not loaded: " << t_filePath << "\n";
		return;
	}

	const sf::Texture* l_texture = getTexture(t_filePath);

	for (const sf::IntRect& l_rect : sliceIntoRects(t_layout))
	{
		m_variants[static_cast<int>(t_type)].push_back(TileVariant{ l_rect, l_texture });
	}
}


void TileVisuals::registerDefaults()
{
	/// ToDo?: Add to a TileType Compendium - Feed in relevant paths and run them all together
	/// Is potentially redundant currently as I don't plan to have several sprite sheets conjoin
	/// Outside of testing, but it is something I wanted to be functionally doable easily
	
	// FilePaths for SpriteSheets used
	const std::string l_grassPath = "ASSETS\\IMAGES\\GrassTest.png";
	const std::string l_stonePath = "ASSETS\\IMAGES\\RockTest.png";

	// Note and reminder of Layouts
	//					rows , columns , Sprite Size , Space between Cells , The Offset to reach the sprite
	GridLayout l_grassLayout{ 3, 3, sf::Vector2i(64,64),  sf::Vector2i(16,16), sf::Vector2i(16,16) };
	GridLayout l_stoneLayout{ 5, 5, sf::Vector2i(40, 40), sf::Vector2i(8, 8), sf::Vector2i(16, 16) };


	// Assigning Defaults
	registerTileType(TileType::Grass, l_grassPath, l_grassLayout);

	registerTileType(TileType::Stone, l_stonePath, l_stoneLayout);


}

sf::IntRect TileVisuals::getVariantRect(TileType t_type, int t_variantIndex) const
{
	const TileVariant* l_variant = getVariant(t_type, t_variantIndex);

	if (l_variant == nullptr)
	{
		std::cout << "[TileVisuals] No variants registered for this TileType. \n";
		return sf::IntRect();
	}

	return l_variant->m_spriteRect;
}

std::optional<sf::Sprite> TileVisuals::buildSprite(TileType t_type, int t_variantIndex, int t_rotationStep) const
{
	const TileVariant* l_variant = getVariant(t_type, t_variantIndex);

	if (l_variant == nullptr)
	{
		return std::nullopt;
	}

	sf::IntRect l_rect = l_variant->m_spriteRect;
	// Ensures there is actually a value being returned - catches empty sf::IntRect() return values
	if (l_rect.size.x <= 0 || l_rect.size.y <= 0)
	{
		return std::nullopt;
	}

	sf::Sprite l_sprite(*l_variant->m_texture);

	l_sprite.setTextureRect(l_rect);
	l_sprite.setScale(sf::Vector2f(static_cast<float>(Globals::TILE_SIZE) / static_cast<float>(l_rect.size.x),
								static_cast<float>(Globals::TILE_SIZE) / static_cast<float>(l_rect.size.y)));
	l_sprite.setOrigin(sf::Vector2f(l_rect.size.x / 2, l_rect.size.y / 2));
	l_sprite.setRotation(sf::degrees(static_cast<float>((t_rotationStep % 4) * 90)));

	return l_sprite;
}

int TileVisuals::selectVariant(TileType t_type, int t_x, int t_y, uint32_t t_seed, const HashNoiseGenerator& t_generator) const
{
	auto l_it = m_variants.find(static_cast<int>(t_type));

	if (l_it == m_variants.end() || l_it->second.empty())
	{
		return 0;	
	}

	int l_count = static_cast<int>(l_it->second.size());
	float l_value = t_generator.generate(t_x, t_y, t_seed + HASH_SALT_VARIANT);

	return static_cast<int>(l_value * l_count) % l_count;
}

int TileVisuals::selectRotation(int t_x, int t_y, uint32_t t_seed, const HashNoiseGenerator& t_generator) const
{
	float l_value = t_generator.generate(t_x, t_y, t_seed + HASH_SALT_ROTATION);

	// Value returned is between 0 and 1, multiplying by 4 open up 0 to 3 as truncate values
	// Modulo 4 is a safety net to ensure we dont get an invalid rotation
	/// Returns; int value between but including ( 0 - 3 )
	/// Use case - assigning angular rotation of a sprite currently 
	l_value = static_cast<int>(l_value * 4) % 4;

	return l_value;
}

const TileVariant* TileVisuals::getVariant(TileType t_type, int t_variantIndex) const
{
	auto l_it = m_variants.find(static_cast<int>(t_type));

	if (l_it == m_variants.end() || l_it->second.empty())
	{
		return nullptr;
	}

	// Prevents overcasting - e.g. havve only 10 sprites, attempting to grab 15 sprite will return element 5
	int l_index = t_variantIndex % static_cast<int>(l_it->second.size());

	return &l_it->second[l_index];
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
