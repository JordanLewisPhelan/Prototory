## Day One
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

## Day Two
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
## Day Three
An ([[Idea 1]]) popped into my head as I was working here today that is certainly too soon to make a call on but would be quite interesting.
Sadly this will be scrapped likely and just fixed, but it will remain like this until I make a RockTest image sheet. So around now and maybe later expect some terrain to look a little wonky as I combat the urge to want to add more complexity.


