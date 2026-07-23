## Session One
Initially started with going in with the idea of a full Visualizer Constructor as it made sense. 
But beginning to write and even name classes I began doubting it this was the right direction as it is not super logical to tie all visuals to one class.

Idea: Was 1 class to derive formatting, for tiles inputs become variants; for UI like inventories they become sequences.

But then I'd have to use a boolean in a constructor or make 2 constructors to just begin a process and that can work but for something that is 2 different directions it actually doesn't make sense in the long run.

So ([[Pivot 1]]) has already occured.

Added in files for both visual classes, but focus for next while will be tiles as they are more critical. 

Added in debug or testing tiles - may replace but hard to find decent quality art that matches and Artist talent is not exactly a quality I have honed. Expect not the greatest art even if remade.

Wanted to make them stand out *enough* but not too much that it became time consuming. 

I decided to make 9 potential sprites per terrain type right now(except iron). This is a lot but I actually have done this ahead of time because I'd like to tinker with elevation influences for the tiles, and until that is added I can script the logic to use all 9 variants, much easy to see how noise is effecting it when I get there. 

Added in TileVisual Setup, tested renderDebug in player(just needed to see it was rendering). NOTE: Removed this before pushing cause it was never meant to stay there but just explaining where debugging occurred.

Was a very nothing day in terms of progress but all the data I collected and basic sprite work done, while literally the bare minimum was done for coding the setup and theory was tested, i just need to make the tile logic next.

---

## Session Two
Today had decent progress but I have not enough time to finalize and test building sprites yet. 

Made a slight change, not really Pivot or note worthy but I updated texture as a sole texture space to a map. I done this because I wanted to allow custom sprites and setups and it also makes breaking up textures easier, I can simply make a Grass or Sand or Stone texture image and use that whole file but keep their dimensions small.

So instead of an atlas or a god sprite sheet with everything bundled together I chose to spread them out. 

This did mean I had to tweak the system slightly from plans to accomodate, but it was nothing major as this was always the plan, I just didn't expect so many minor hitches to crop up.

There was a lot of issues ones I hadn't even considered until working on it actively, the initial test texture i thought would be enough to test the pair I have added in this commit, rock and grass, but no, Due to the prior issue of separate images and overwriting this wasn't feasible long term, this would certainly cause issues;
Another issue was having to worry about identifying how to account for different image/sprite images would be spaced out or handled, forcing all images to be 3x3 - due to my test examples, would be catastrophically limiting. 
I went with the choice that made the most sense, A simple struct that Holds basic data about how many rows or columns to partition and skim through. 

I didn't realize it initially but this does have some overlap with how I could setup the inventory system, I had envisioned using the same system before to construct UI and Tile Variants but by complete coincidence I have the segment that applies to both as a struct and can focus on working on both in time.

Inventory is a ways off of course but it just intrigued me that while I missed the implementation my idea had merit, I just had to extract the right parts. Will attempt to use this when I work on the inventory UIs.


I have added this work into Gameplay, this is because the work above made sense and was made easier by referring to my old work, the Definitions for machines and resources, the idea is scraped from there, assign everything in a local hotspot and register them all when needed.

This push will not have any sprites showing but I have tested that the images load correctly.

The next day will consist of trying to implement them to tiles. 
That was meant to be today, but alas it was a bigger job debugging and planning than I thought so will be progressing with optimistic caution.

---
## Session Three
An ([[Idea 1]]) popped into my head as I was working here today that is certainly too soon to make a call on but would be quite interesting.
Sadly this will be scrapped likely and just fixed, but it will remain like this until I make a RockTest image sheet. So around now and maybe later expect some terrain to look a little wonky as I combat the urge to want to add more complexity.

Wrote beyond here after this day - Basically backfilling what I had done and why to the best of my recollection.

Added in buildSprite function to make and display a Sprite Variant and return that sprite. It uses std::optional as I wanted to protect against moments during debugging where a Sprite may not exist or for later on in case it has no sprite whether i forget to give a sprite or under the guise of extensibility, it doesnt break if I were to attempt to add a new sprite type and I apply it incorrectly, those tiles would not have a sprite and errors would occur.

Documentation on how ([[BuildSprite()]]) is used and handled is in the respective linked md file under Documentation.

Built on buildSprite to add in rotating the sprite to add more randomness. This is simple overall but I wanted to make sure this worked, so something that could have been ; sprite.setRotation(value) - Had to take up some attention I didnt really plan to do today but when something feels simple its a lot easier just do it.
Sadly that meant I had to add in another feature;

Deterministic Selection for Sprites: This was initially to organise the sprites rotation. But what makes it deterministic is I had to tie it to the seed somehow. 
I was stumped but it dawned on me I always had plans to reuse the Noise Generators, and this was only a simple number selection so i decided to use the HashNoiseGenerator for this and just made some salt values - copied logic from WorldGenerator - Holding onto that old code in comments worked out! Even if not used 1 to 1.

I do have some gripes about how I designed it this commit, it feels weird to pass in a full generator, even if it already existed and is simply a reference in my case. I'd like to go back later if I have time and find a more light or atleast not parameter cramming method of doing the same thing, but it is proven to work! So that is key takeaway from this commit, it panned out and the seed does influence the tiles sprite and random rotation, which is what I wanted.

Those were the only additions but of course I had to update the Generation process to add this logic in. 

Updated TileMap and WorldGenerator and Im pretty sure ChunkManager to accept TileVisuals reference to apply the changes.

A neat but welcome boon was I was worried about how the rendering of TileMap would act if I was to add the Sprites, sprites loaded forever isnt exactly safe culling, But the design of attaching the loaded sprite via VariantIndex and the rotation made this relatively cheap considering its sprite building. Only draws and builds tiles and sprites that are near the player.
It was welcome as it worked out of the box after adding those variables to Tile and updating the values when generated.

NOTE: Still not entirely sure about the frame hitching, will explore on the next full working day.


## Session Four
Today was small but very important. Done a mild refactor to the TileVisuals setup, moved from trying to force textures and types to be synchronous and bound to being a little more flexible. As I discovered from earlier, there was an Overwritting issue when calling TileType::Grass twice, but trying to add on another sprite sheet, example, rocks can sparsely appear in plains biome as a use case example. 

The issue was this was overwriting the previous bound filePath so I had to change how that was registered and assigned slightly.

The only thing really left for this feature is adding in the remaining TileType sheets and to add in IronOre - I actually forgot to make that before.

I have some wishes I would like to add to this, such as a Compendium, throw in all file paths there and either organise a delegation system when passing in, e,g. an unordered map like these variants can be used by Sand and Stone tiles, while this other list can be sand and water tiles, I think this would start simple and scale up into visual deviations like how I wanted certain tiles a certain colour based on depth, this is still present but isn't functionally implemented with sprites.


**ADDENDUM:** There is 1 additional feature I added all this for and nearly forgot about adding;
Depth Gauging Terrain - Basically if terrain is too steep on a certain side a visual will come alongside that to reflect that you cant scale it.

Next session will probably consist of Tidying up loose ends with the TileTypes being set up and adding in IronOre sprites.

Followed by work starting on the Depth Gauging - Unsure what to call it, will workshop it when I look into researching it. 
**BONUS NOTE:** Should be able to use bitmapping setup, it should be able to work well as should be quick enough to do during generation to update and join terrain.
Maybe orchestrating a full bitmap system and rendering the sprites based on ruled pairings? Would be a big shift but would tie sprites visually together seamlessly.
Will be considered but focus will be on bitmapping or simply exploring possibilities pertaining to determining directional steepness.

**Closing Note:** Also changed Day 1,2,3 headings into Sessions - between research and other unavoidable breaks some workflow has been split between commits, 'Day' is an inaccurate portrayal of the process.


## Session Five
~Prepped~

