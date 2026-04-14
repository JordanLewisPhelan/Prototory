#pragma once
#include <cstdint>

class INoiseGenerator
{
public:
	~INoiseGenerator() = default;

	
	
	
	
	
	
	
	
	virtual float generate(int t_x, int t_y, uint32_t t_seed) const = 0;
};