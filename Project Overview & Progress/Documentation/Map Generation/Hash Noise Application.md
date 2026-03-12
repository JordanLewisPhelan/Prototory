
This Document will go over Hash Noise and the effect it has on the Project.

Currently(12/03/26) Hash Noise is unused, it was my first implementation of utilizing noise and the Interface for applying noise, the idea was to experiment with simple and random, but not recurring; progressive noise.

This as someone described to me has a "TV Static" noise type, where it is blocky and incohesive, it simply is random number generation and the logic here is simple and minimal. 

The current plan is to use this to in time Generate the Biome/Chunk system based on the seed of the world, then use more cohesive noise like Perlin Noise or FBM noise to apply to the Map itself and the tiles present there. This means even when biomes merge, cohesion and logic should still dictate influences. This will make map generation feel believable yet not impossible to maintain on the backend.