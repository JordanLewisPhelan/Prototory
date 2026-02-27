#pragma once

#include "INoiseGenerator.h"


class HashNoiseGenerator : public INoiseGenerator
{
public:

/// <summary>
/// Generates Simple Hash noise for a specified tile.  
/// Uses: 
/// 	Current: Good for testing and ensuring Noise Applies correctly through Interface Hub
/// 	Future: Biome inner logic or chunking Noise - controlled chaos
/// Consistencies Explained in INoiseGenerator Summary
/// </summary>
/// <param name="t_x"> Specified Tile X coordinate </param>
/// <param name="t_y"> Specified Tile Y coordinate </param>
/// <param name="t_seed"> Map seed </param>
/// <returns> float value of the Hash Value generated, used for determining tile type & cohesion </returns>
	virtual float generate(int t_x, int t_y, uint32_t t_seed) const override;
};


/*
 ToDo/Consider:
	Construct actual cohesive documentation? More Info than function. 
*/