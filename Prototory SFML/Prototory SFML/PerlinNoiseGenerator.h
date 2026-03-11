#pragma once

#include "INoiseGenerator.h"
#include <vector>
#include <cstdint>

class PerlinNoiseGenerator : public INoiseGenerator
{
public:
    PerlinNoiseGenerator();

    float generate(int t_x, int t_y, uint32_t t_seed) const override;

    // Octave & Influencer setters
    void setFrequency(float t_frequency) { m_frequency = t_frequency; }
    void setOctaves(int t_octaves) { m_octaves = t_octaves; }

private:
    std::vector<int> m_permutation; // Permutation table (512 elements)
    float m_frequency;              // Default (0.05): Scalar for tile type frequency which is applied to the Tiles
    int m_octaves;                  // Default (1): Layer of octaves for when FBM or layered noise is introduced

    /*- Static Values for Permutation Table -*/
    static constexpr int PERM_SIZE = 256;
    static constexpr int PERM_MASK = PERM_SIZE - 1;


    // Initialize permutation table with seed
    void initializePermutation(uint32_t seed);

    // Noise function for a single tile
    /// When adding FBM will need to be adjusted and updated to match
    float noise(float t_x, float t_y) const;

    // Helper functions
    float fade(float t_factor) const;
    float lerp(float t_factor, float t_startVal, float t_endVal) const;
    float gradient(int t_hash, float t_x, float t_y) const;
};