#pragma once

#include "INoiseGenerator.h"
#include <vector>
#include <cstdint>

class PerlinNoiseGenerator : public INoiseGenerator
{
public:
    PerlinNoiseGenerator();

    float generate(int t_x, int t_y, uint32_t t_seed) const override;

    
    void setFrequency(float t_frequency) { m_frequency = t_frequency; }
    void setOctaves(int t_octaves) { m_octaves = t_octaves; }

private:
    mutable std::vector<int> m_permutation; 
    mutable bool m_permutationInitialized{ false };
    mutable uint32_t m_cachedSeed{ 0 };

    float m_frequency;              
    int m_octaves;                  

    
    static constexpr int PERM_SIZE = 256;
    static constexpr int PERM_MASK = PERM_SIZE - 1;

    
    void initializePermutation(uint32_t t_seed) const;

    
    
    float noise(float t_x, float t_y) const;

    
    float fade(float t_factor) const;
    float lerp(float t_factor, float t_startVal, float t_endVal) const;
    float gradient(int t_hash, float t_x, float t_y) const;
};