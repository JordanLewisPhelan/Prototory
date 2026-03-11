
Currently as of 11/03/26; 

The Map Generation is a whole system, it is not the main piece but I wanted it to be a showcase of technical effort in itself even if it is not fleshed out fully by the end of the project.

I wanted to use applications of Noise to generate believable but not predictable terrain. At this time Perlin Noise even at a basic level - I do want to scale it up Once or Twice(I will discuss this later), is actually quite sufficient, it ensures cohesion without making everything predictable or identical outside of Seeds - Yes for debugging I wanted to add a consistent frame of reference. This will make adding new content be consistent, and if something breaks we have narrowed down where the issue is immediately.

I would like to experiment with FBM(Fractal Brownian Motion) noise either to design or in tandem with a Biome or Chunking system, this way there is deviations without a certain element dominating the world - Currently Grass tiles actually take up a large majority of the world, there is some of every tile in the world even now but it is overwhelming how much Green there is.

I want this world not to feel alive(Mostly as its going to be coloured squares which isn't life-like), But to feel believable enough for you to want to make do with what you have. To plot *Your Own World*.  