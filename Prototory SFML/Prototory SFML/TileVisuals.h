#pragma once
#include "Globals.h"
#include <vector>
#include <cstdint>
#include "Tile.h"


struct TileVariant
{
	sf::IntRect m_spriteRect;
};


/*
Simple Struct to hold data to map out how sprites should be captured to be used as variants
Debating using this to also make a grid for UI, like same parameters would be needed so will test later on
*/
struct GridLayout
{
	int m_rows = 0;
	int m_cols = 0;
	sf::Vector2i m_cellSize;	// Space to capture an image to display
	sf::Vector2i m_spacing;		// Spacing between mapped cells
	sf::Vector2i m_offset;		// Initial offset for image capture, e.g. rarely will an image be in top left corner(0,0)	
								// Used to get starting point
};


// Maps to a GridLayouts coordinates to slice a rect into an image
std::vector<sf::IntRect> sliceIntoRects(const GridLayout& t_layout);


class TileVisuals
{
public:
	bool loadTexture(const std::string t_filePath);

	const sf::Texture* getTexture(const std::string& t_filePath) const;

	const sf::Texture* getTextureForType(TileType t_type) const;

	// Sets up TileType variants - same concept from the definitions before; Machine/Resources
	void registerDefaults();

	// Returns Variant of : Type , index, e.g. Grass, 4 will give 4th grass variant 
	sf::IntRect getVariantRect(TileType t_type, int t_variantIndex) const;

private:

	std::unordered_map<std::string, sf::Texture> m_textures;
	std::unordered_map<int, std::string> m_typeToTexturePaths;
	std::unordered_map<int, std::vector<TileVariant>> m_variants;
};