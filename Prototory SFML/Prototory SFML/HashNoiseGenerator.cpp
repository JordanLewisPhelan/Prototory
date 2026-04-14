#include "HashNoiseGenerator.h"

float HashNoiseGenerator::generate(int t_x, int t_y, uint32_t t_seed) const
{
	
	
	uint32_t l_hash = t_x * 374761393 + t_y * 668265263 + t_seed;

	
	l_hash = (l_hash ^ (l_hash >> 13)) * 1274126177;
	l_hash = l_hash ^ (l_hash >> 16);

	
	return static_cast<float>(l_hash & 0xFFFF) / 65535.0f;
}