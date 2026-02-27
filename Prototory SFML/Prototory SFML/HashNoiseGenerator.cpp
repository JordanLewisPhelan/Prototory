#include "HashNoiseGenerator.h"


float HashNoiseGenerator::generate(int t_x, int t_y, uint32_t t_seed) const
{
	// MurmurHash Algorithm - Hence Large Prime Numbers
	/// Mixes and combines X and Y Tiles and seed to create large (rarely) pattern forming sequences
	uint32_t l_hash = t_x * 374761393 + t_y * 668265263 + t_seed;

	// Mixing element - Uses MurmurHash bit operations
	l_hash = (l_hash ^ (l_hash >> 13)) * 1274126177;
	l_hash = l_hash ^ (l_hash >> 16);

	// Extracts the latter 16 bits(which we just compressed to) and normalize between [0.0 and 1.0]
	return static_cast<float>(l_hash & 0xFFFF) / 65535.0f;
}