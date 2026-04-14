#pragma once

#include "INoiseGenerator.h"

class HashNoiseGenerator : public INoiseGenerator
{
public:

	virtual float generate(int t_x, int t_y, uint32_t t_seed) const override;
};

