
This document is going to refer to how to utilize the Noise Generation in terms of Perlin Noise within this project to influence and adjust the Map Generation as you need.

There are 3 topics I will be covering that are touched on in the code but may be left to interpretation if just reading directly and may result in confusion when trying to adapt it for use within this Project.

1. Variables & Naming Conventions
2. Bitmasking operations & Permutations
3. Function Purposes & Processes Explored


#### **Variables & Naming Conventions**
Throughout the project "L_" is used to determine that a value is Local to a function unless it is used only to return a value. 
Mentioning this as you will see it a lot everywhere but in clumps here in these functions.

Otherwise we will look at the Variables and their purposes and what they influence with more depth:

m_frequency:
This is what spurred this whole topic; this is used to determine how big or small a tile formation should be or how common they are.
Example: default is 0.05 out of 1, this means how far it will expand or rather how it will increment through the Map Generation, it is set up to be between 0 and 1, the idea is tiles that have a lower value are more common because their shallow increments leads to a more gradual but wider spread, so there is more of them.
 Where-as 0.1 is quite large, you will quickly find that only a very limited amount can spawn in an area because of the incremental effect, this leads to larger numbers == smaller surface area covered.

m_octaves:
This is currently unused as of writing but will function for FBM(Fractal Brownian Motion - add reference later) where will we make more refined Noise by passing this many times, 1 is the default, but we could do 4 passes and this will refine the Noise even more
ToDo: Update this with more specifics after FBM is added, this is too awkward to explain without full scope and may be wrong if i forget to update any changes.

#### **Bitmasking Operations & Permutations**
Basically we use bitmasking to increase performance. It is simply a trick to get a modulus operation for cheap, we can only do this for values at the power of 2, e.g. 2^8 or 2^10, that is why we have those values. 

We can absolutely replace this with "% 256" and have the same results.
But I chose to try it this way as I feel there is more that can be done here and engaging with it is how I will learn, in its current form this is basically just a performant modulus operation.

The reason we do this however is for the Permutation Table. The trick with this and why it comes up during Perlin Noise is we are going to use this as a "Look-up table" for directional changes and as such directional gradients. This is much smaller than the whole map which is currently 16000~ tiles. This choice was made in this way to ensure we arent querying the full map, just a smaller sample size - i.e. performance.

#### **Function Purposes & Processes Explored**
This will be long as ill go over each Function and explain it briefly, but with some depth.

Functions to Cover:
**float generate(int, int, unsigned int)** - Interfaced function all Noise Generators use, but altered to generate more succinctly with Perlin Logic
**float noise( int, int )**  - Will be adapted during FBM addition
**void initializePermutation(unsigned int)**

Helper Functions;
**float fade(float**)
**float lerp(float, float, float)**
**float gradient(int, float, float)**


**generate():**
This functions purpose currently is to generate Noise for each tile individually. This will be updated later on when we add chunking as then it will functionally still work but will need some tweaking.

The process is we begin by using the unsigned int which is actually our Seed for the World and initialize the Permutation Table for use later. 
For clarity - this is not ideal, we do not want to actually do this for each tile which is how this currently works. This is a simple fix as it needs to run, but it simply should not run on each tile check.

After that we use the m_frequency to scale the tile coordinates being our X and Y of the tile we are investigating and getting the noise for. Then using those scaled X and Y values we pass it into Noise which will give us a Perlin Noise value of between -1 and 1. 

The only thing we do now is return the noise value we just got, however, our system needs values between 0 and 1, not -1 and 1, so we simply quickly scale them up by adding 1 and dividing by 2, this remaps our value to be inline with the new {0, 1} return value.
This isnt perfect, i have also tried absoluting the value but this had an interesting effect, it actually made Water a very dominant tile. 
So currently the idea is leave it as is and revisit when we have chunks, this way we can have more control over what can spawn most commonly without depreciating other tiles due to math.

return value: float; Noise value of tiles between {0 and 1}

**noise()**
This functions realistically just generates the pure algorithm of perlin noise without a care for what that means for the world.

Process wise we begin by checking the tile we are in, we floor them so we have them just the permutation indexes as we dont need to know where we are inside the tile specifically. We use the bitwise operation again here to ensure we get a value that the Permutation table will accept as valid, e.g. we are in tile index(400, 238) when we add in procedural world generation mid game for example, this way the tiles will always have a valid permutation index and wont cause issues or corruption. 
Next we figure out what part of the tile we are in - l_x/yFloor is investigating where inside the tile we are as this will be used for the fade() function, this is used for blending into other tiles and primarily what are the tiles surrounding influences on the Permutation Table in that area. Tile Indexes may have different influences than the center of that same tile, so we need to account for that or we may get bias generation.

Next we determine where we are in the permutation table using the floored values we acquired earlier X-Y indexes of the tile. We then map out where in the permutation grid that is and get a surrounding view, these are mapped out as topLeft/Right and bottomLeft/Right. 
We use these permutation positions with our gradient function and the tile position to check where there are (if any) bias', we use the returning values of each corner to simplify it further, we get a lerped value of the top and bottom X axis' and use both of those values to interpolate along the Y axis - the values combined are the Perlin Noise we return to generate().

return value: float; scalar value of tiles Noise between {-1 and 1}



**initializePermutation()**
This function seems complex but really it is just randomly assigning values to an array(permutation table) and doubling them at the end of the array as the Permutation Table should always be twice or thrice the size to account for the Dimensions we are working with - in this case 2D; X and Y so doubling the size is plenty. 

We do this so there is always a value for the Noise to use and it never goes out of the arrays bounds when calculating the size of the permutation map and where edges are but we also get a benefit of Perlin Noise value not being influenced by tiles outside of the tile we are generating noise for, as such we dont need to worry about out of bounds tiles influencing in bounds tiles.


**fade()**
This function uses the Quintic Polynomial(Add a reference later) equation to create an S curve while not leaving any derivatives at edges. 
In essence we make a gradual S curve, that starts slow and accelerates and slows down after reaching the mid point before slowing again before coming to a stop. 
In this project this is used to find the smoothest curve for a tile which is used when we interpolate along the X and Y axis for the Noise. 
This is used when we are translating from the Permutation table, we use this smooth curve influence to enforce that a close edge will have more of a pull in that direction, this will prevent things being blocky or like a line segment was cut into the terrain. I.e. less visually obvious there is a pattern.

return value: float; returns the smoothed input value between 0 and 1 

**lerp()**
This function is simply Linear Interpolation, we give it a factor to move to and a start and end point and we smoothly move from the start to the end position.
In this project this is used to find interpolate between the X and Y axis' which are using the gradient influences at each corner, i.e. What is the noise using the extremes.

return value: float; returns the input(Factor) percent between startValue and the endValue position

**gradient()**
This function is used to get the influence and direction vector of our tiles use bit operations. 
We start determining what we will actually need from this, which in this case is one of the 12 gradient vectors that is commonly used for perlin noise.  So in this case we know we only need the 12 at most but we want to use bitwise operations to break down what we are working with. So we grab the bottom 4 bits ;
(t_hash(input value) & 15 ). 

Following that we begin the calculations which we feed into floats u and v(just naming conventions for the process i do wish there was a better descriptor). 
We start by determining the primary component/axis, if the hash value is below 8, X is used, if it is above 8 we use Y.
This helps determine the stronger direction which is important for determining where our gradient is facing.
We check other specified cases then where we check if the hash is below 4, if so we use Y, otherwise we use  X or 0. This is determined by checking the 2 special cases of 12 and 14, there we use X, all other option use 0 which means there is no diagonal component, we are on the axis.

Notice: This isnt perfect as we have wasted bit space, there is better more accurate methods, but this is just a usable variation of the gradient implementation
The idea is here with this implementation we sacrifice optimal usage for fast usage. 
Refer to ***Perlin Noise Influences*** in[References]

return value: float; We use 1 last bit check to determine what (U,V) should be in this case, e.g. are they negatives or positive, if bit 1(U) comes back 0, we know it is positive, otherwise if it is 1 we know its negative, the same applies to bit 2(V).



Why this was chosen: Perlin Noise is a tried and tested formula and I knew I would need it for when I begin working on larger models, such as Fractal Brownian Motion, it is also awkward because a lot of the time you don't hear about how much goes into Noise functions, they always seem very simple, but this is still being worked on and adapted(Even if this set is work from tested 2000s data) there is always work to be done.
So I chose to engage in this was because I believe understanding what goes into these processes - even if I dont understand all the nuances, e.g. why Quintic Polynomial is SO much better, I do believe in results. Also I believe in trying to understand things so I can make my own adaptations without breaking the core it was built upon.