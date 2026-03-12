#include "PerlinNoiseGenerator.h"
#include <algorithm>
#include <random>
#include <cmath>

PerlinNoiseGenerator::PerlinNoiseGenerator()
    : m_frequency(0.05f)  
    , m_octaves(1)        
{
    // Pre-size to account for the full length of the permutation table
    m_permutation.resize(512);
}

float PerlinNoiseGenerator::generate(int t_x, int t_y, uint32_t t_seed) const
{
    // *ToDo: Remove seed parameter when Scaling Systems up; e.g. ChunkManager is implemented.
    // Seed ownership should belong at WorldGenerator level, not here in generators.
    
    // Initialize permutation table if seed changed - Temporary
     const_cast<PerlinNoiseGenerator*>(this)->initializePermutation(t_seed);

    // Apply frequency to scale coordinates
    float l_scaledX = t_x * m_frequency;
    float l_scaledY = t_y * m_frequency;

    // Single octave for now (add FBM logic here later)
    float l_noiseVal = noise(l_scaledX, l_scaledY);

    // Perlin noise returns [-1, 1], so this converts to [0, 1]
    return (l_noiseVal + 1.f) / 2.f;
}

void PerlinNoiseGenerator::initializePermutation(uint32_t t_seed)
{
    // Create base array
    std::vector<int> l_permTable(PERM_SIZE);
    for (int i = 0; i < PERM_SIZE; ++i)
    {
        l_permTable[i] = i;
    }

    // randomize based on seed
    std::mt19937 l_rng(t_seed);
    std::shuffle(l_permTable.begin(), l_permTable.end(), l_rng);

    // Duplicate at end of base array to align with permutationTable size
    for (int i = 0; i < PERM_SIZE; ++i)
    {
        m_permutation[i] = l_permTable[i];
        m_permutation[PERM_SIZE + i] = l_permTable[i]; // Duplicate
    }
}

float PerlinNoiseGenerator::noise(float t_x, float t_y) const
{
    // Find grid cell coordinates
    int X = static_cast<int>(std::floor(t_x)) & PERM_MASK; // Wrap to [0, 255]
    int Y = static_cast<int>(std::floor(t_y)) & PERM_MASK;

    // Find relative position within cell [0.0, 1.0)
    float l_xFloor = t_x - std::floor(t_x);
    float l_yFloor = t_y - std::floor(t_y);

    // Apply fade curves to smooth interpolation
    float l_smoothX = fade(l_xFloor);
    float l_smoothY = fade(l_yFloor);

    
    // Hash coordinates of the 4 grid corners
    int l_topLeft = m_permutation[m_permutation[X] + Y];     
    int l_topRight = m_permutation[m_permutation[X] + Y + 1]; 
    int l_bottomLeft = m_permutation[m_permutation[X + 1] + Y];     
    int l_bottomRight = m_permutation[m_permutation[X + 1] + Y + 1]; 

    // Calculate gradient influences at each corner
    float l_gradientTopLeft = gradient(l_topLeft, l_xFloor, l_yFloor);     
    float l_gradientTopRight = gradient(l_bottomLeft, l_xFloor - 1, l_yFloor);     
    float l_gradientBottomLeft = gradient(l_topRight, l_xFloor, l_yFloor - 1); 
    float l_gradientBottomRight = gradient(l_bottomRight, l_xFloor - 1, l_yFloor - 1); 


    // Interpolate along X axis 
    float lerpTop = lerp(l_smoothX, l_gradientTopLeft, l_gradientTopRight);    // Top edge
    float lerpBottom = lerp(l_smoothX, l_gradientBottomLeft, l_gradientBottomRight); // Bottom edge

    // Interpolate along Y axis 
    float result = lerp(l_smoothY, lerpTop, lerpBottom);


    return result;
}

float PerlinNoiseGenerator::fade(float t_factor) const
{
    // Uses Quintic polynomial - ToDo show & add reference 
    // This creates an S-curve with zero derivative at edges
    return t_factor * t_factor * t_factor * (t_factor * (t_factor * 6.0f - 15.0f) + 10.0f);
}

float PerlinNoiseGenerator::lerp(float t_factor, float l_startVal, float l_endVal) const
{
    // Linear interpolation
    return l_startVal + t_factor * (l_endVal - l_startVal);
}

float PerlinNoiseGenerator::gradient(int t_hash, float t_x, float t_y) const
{
    // Use hash to select one of 12 gradient vectors
    // Then compute dot product with offset vector (x, y)

    int l_hash = t_hash & 15; // Use bottom 4 bits only - why we use 15

    // Convert hash to gradient direction
    // This creates 12 evenly distributed unit vectors
    float u = (l_hash < 8) ? t_x : t_y;  // If hash < 8, use x, else use y
    float v = (l_hash < 4) ? t_y : ((l_hash == 12 || l_hash == 14) ? t_x : 0.0f);

    // Apply signs based on hash bits
    return ((l_hash & 1) == 0 ? u : -u) + ((l_hash & 2) == 0 ? v : -v);
}