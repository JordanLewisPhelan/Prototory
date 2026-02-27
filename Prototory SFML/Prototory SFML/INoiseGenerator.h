#pragma once
#include <cstdint>


// Interface for Noise Algorithms - Plan to allow for multiple and mix-match
class INoiseGenerator
{
public:
	~INoiseGenerator() = default;

	/// <summary>
	/// Generates noise range between [0.0 - 1.0], for
	/// a single Tile at a specified index with seed influence
	/// </summary>
	/// <param name="t_x"> Specified Tile X coordinate </param>
	/// <param name="t_y"> Specified Tile Y coordinate </param>
	/// <param name="t_seed"> Map seed </param>
	/// <returns></returns>
	virtual float generate(int t_x, int t_y, uint32_t t_seed) const = 0;
};