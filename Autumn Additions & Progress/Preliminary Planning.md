### *Mental Model* 

The idea for what I want to add is to focus on making something that will be reusable but flexible, this is my One Rule with this and almost all my projects - Even if I do not do things perfectly, the goal will always be to have;  **An Extendible Feature that accepts inputs or concepts and makes that easier to Adjust later, rather than hard coding as much as possible.** 


### Feature: Visualization & Art/Sprite Constructor

This is something I am considering instead of simply adding sprites, that would in my mind, detract from the random generation that is already present, so making a system that uses the generated seed or perhaps that aligns to fit the terrain is the 2 options I'm considering.
	I absolutely could add sprites but they would be very simple and not dynamic; and that bothers me personally. Doesn't mean it wouldn't work but would break my One Rule for this project, because i'd be hard coding.
I also wanted to do this as it would resolve a big issue with the game feeling hard to perceive.
#### **Requirements:**

 Choosing an Angle: Seed focused randomization of sprites/art
or
Terrain fitting, e.g, gradual tile 'tainting' leaking into other tiles or influencing where rock or mountain tiles will be invaded by grass tiles if they are adjacent. 
   
   Explanation of choices:
   - Seed Focused Randomization means I can use any tilemap or backdrop or logic and segment them in some way and that is persistently useful, basically can drop in a bunch of available tiles and rules apply them to the right biomes. But if I want to add or change something its as simple as changing the tilemap and updating it, could even be done at runtime in some cases if that was needed - but that is only conceptual as an example.
   - Terrain Fitting would be logical and smoother as I already have gradient fitting logic due to perlin noise, I can reapply it to this and test and debug it to ensure that with some checks there will be smooth transitions in tiles, this is an option because it already exists partially, but will also be scalable as I simply would have to make rules and maintain them.
   Both are good, but choosing one is critical - As of typing I am leaning more toward a tileMap Seed Focus.
   
1. Terrain Fitting format addition; would need to simply be aware of its actual neighbour not just the gradient from perlin as we would need to still understand what we are connecting to, to see what variation of connection we should use. This does have 1 glaring issue; We would need to make sprites for all of the potential hybrid or merging scenarios and each gradual change, such as high mountain rocks will be brighter than low mountain rocks. This would be hard to automate and so doing it manually and assigning it would be the best option, at which point its not as ideal because we have to plan for all updating cases to represent the changes. We could code the visuals which is part of what will be done but I cannot mentally view that just yet so would need some time and reflection to gauge the pros and cons of this format and how to make it functional.
2. TileMapping format addition; need to convey that information and randomise that in some way, could be a grass tileset, grass on the left, bottom, right, some tall grass. Focusing on keeping it simple to accomodate the bare minimum and having a readable tileMap or maybe a subset of potential sprites/image to turn into sprite
   
   **Note: Thinking as I type, Skip unless want clarity**<Could still use the PerlinNoise to add some cohesion here for lets say tall grass, but that is conjoining other logic, I think simple is a little better and simply adding variations of the potential grass tiles in this case would be sufficient, rather than full tile variations like Tall grass that may need more logic.> 
   
![[ExampleTileset.png]]ExampleTileset.png : If too hard to read; Example shows rough idea of multiple similar but different tiles to use to fill in grass or rock tiles in this case.


- Want to allow and permit other ideas too, for example, what if I wanted to make the Inventory Visualization? Want to make it easy to construct once this is added, so could use this groundwork to add onto that more fluidly. 
  In this case could make it so there are components to a visual - this idea exists for tiles already, TileVisual. 
  Could update that to use the logic made here and use that to display the right sprite.
  
  **Note: Simply typing this parts has made me realize the Option for Terrain Fitting would be too rigid as that would only work there, that is NOT visualization work at that point and is a tile decision, Leaning towards the TileMap example more and more.**

-  Item Visualizers, simple icons for like wood, or stone resources and can be applied to the resources directly from here? Perhaps maybe not... could also be a new element for the ResourceManager to hold - IconSprite.. would be simpler.. good to note but not really for this class - UNLESS pairing visuals to data driven focuses is harder than expected.. this is important to investigate but could be super simple.

### Feature: Inventory Visualization & Transfers(Potentially)

As I go over in Visualization Constructor we could use this in varying ways but for Inventory Visualization I am quite intrigued in using a map or key-value dictionary of some kind. 

Text example: 2 values;
Key = Element(0), Value = Inventory Visual.
Imagine Leons Briefcase in Resident Evil 4 but as a "background" for player inventory, but can be different for a machine, conveyors could have a simple conveyor background, Auto Quarries can have like a Drill or excavator background.

Key = Element(1), Value = InventorySlot Visual.
Simply a shape overlay, like a black box, or a gray box outline, again for player think Resident Evil 4 briefcase inventory, empty slot have a gray background and outline of the slot, filled items just have icons.


-  Will need to add in key inputs and *potentially* drag and drop between inventories, player and machines or storages.



### Final Choice

After typing this out and letting things sit for a few days, I have decided, the TileMap format where a List or Vector of elements that make up potential visualizations will be the Design choice I begin looking to implement.

I choose this as I can have different sprite or art options made and insert those into an "ApplicableVariants" List, made up of what is decided. This is important as if I want to add UI constructs atop the visualization of tiles I can use this methodology to organise how visuals are made

![[ExampleInventory.png|369]]


### Final Preparation & Plans

ToDo:

1. Make a VisualConstructor class that can take in variations and allow for Applying visuals or sprites to Tiles & Ability to make UI constructs that have their own parameters such as the background sprite and how inventory slots will look.
2. Ensure there is dictionaries of some kind paired to this, Will NOT be iterating over every tile or inventory if it can be helped, Apply once at correct time and only to relevant entries - E.g, Mountain terrain can only have rocky formations and variants, not grass or water variations.
   Note: Dictionaries with rules could also allow automation for picking out tile preferences, e.g. Water at lower depths picks darker shades of water, element 4 or 5 perhaps where all deep ocean sprites are.