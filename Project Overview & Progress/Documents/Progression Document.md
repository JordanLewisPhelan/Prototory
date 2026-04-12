 
#### ***Progress Report***
##### Note 1: Written on 7/11/25

So the work mentioned here now is a recollection and reflection of work done prior on the 4th of November. 

So to start I added in a SceneManager. I done this as While Gameplay will be the biggest and core loop we need to allow players to change some options and allow them to quit or save, etc. If the player decides to leave there should be some processes done if they switch back to the menu. Traversing these scenes could be a hassle so trying to organise this early to circumvent technical debt was well worth it.

As a result I also ended up making Buttons to traverse these menus - Did not add it to Gameplay yet but that will be much easier now that the hard work is done. Buttons are a component class and are customizable, will allow me to do quite a bit in terms of controlling menu flow and whatever else i need them to do. I can also certainly reuse some of the button logic later on as I discussed in [[Research Progress]] - The work with Lambda functions has been quite helpful, I understood them but working with them has given me plenty of appliable ideas. I do believe while not a lot was added technically. A lot was done. This has been a productive start. It is not blazing, but I prefer to slow and methodical approach, Over Engineered over Under Cooked.


##### Note 2: Written on 25/2/26

Returning to work(Yes its been a while), Just took today to iron out some details and ensure I was getting reintegrated with the flow - it has been a while even if its just a menu system right now.

Added in a smooth player movement and camera the follows the player atop player being bound to the Map, as an extension the map while still only one type of tile now visually will update as we move around, de-loading and rendering tiles as we will see them on the viewport. What went into this was more than just simple work, it was also some realizations some things may need to be refactored, my ideas worked before but when actually contemplating structure there is certainly going to need to be some adjustments later, currently they are not needed and i have them noted, this will prevent excessive refactoring beyond clean up, I know where will be changed, so I can avoid bloating the games foundation around aspects I am crystal clear are not for the final build but for framework testing.

One additional thing I have decided on today too, I wanted a fleshed out procedural map generation system, I will add some form of procedural generation, but it is likely not to be restrictive in this rendition, focus will be  on getting a baseline to re-use so that I can get to working on the meat of this project sooner rather than later; 

Connection of Systems, automations and resource acquisition and lifetime - acquire, or harvest, deposit, manually or automatically to a set point and flourish from there.

Goal is always automation, even if what is done with the automation is simple or the reason we gather is not fleshed out for this demo, I will be certain to focus and target the Automation and Systemic nature of the games Roots, that's always been the end-goal, i just really wanted a game from this, but plans change, maybe ill even get a game out of this if circumstance allows.


##### Note 3: Written on 27/2/26

Added in Seeded Generation and an Interface to apply Noise Generation. Currently only HashNoise is implemented and functioning but it is scoped for the future.

Seeds are deterministic, ideal for debugging later on. 

Forward looking:
- Loading the Noise makes the game take a few seconds to open after clicking start, Debating pausing the implementation to add visual feedback to let the user know how far loading is or if it got stuck without use of the console. 
- Continuing the Procedural side of work, e.g. Adding more types or Noise for generation and combining them to form Procedural generation.
- To further boost Procedural Work I also aim to add in some details and Biomes so hat there is a system in place that can be outwardly progressed, we can influence and control the rules that will tie tiles together and adding additional attributes to the World - e.g. Water is not walkable by the player so you must go around. 
- Invest in Resource & Inventory work to allow Resource Acquisition. NOTE: this requires a few pre-existing systems and while could be added there is far too much of greater important(structurally) to do before adding in these systems.
- Reorganise Tile System internally - Idea is fine and it works fine for now, but there is a lot of information that will need to be on them, using Switches is fine temporarily but is not ideal in a large scale project in such a pivotal position - The tiles/terrain will impact a lot of things later.

Inner Update:
Not worth a large Note, but added in Loading Screen - Needed to update the sceneManager slightly - it now has a reference to a single Window, SceneManager takes it from Game and then it is delegated downwards - this is done as while there is local windows for scenes such as Gameplay - Modals are separate and will need this Later if I choose to add them - I had an interest to try make modals for reuse, this isn't critical for right now, but will save me exponential time later on.



##### Note 4: Written on 11/3/26

Added PerlinNoiseGenerator.

This was a whole ordeal, the amount of elements that went into this is daunting. I am proud of how it came out, even if it not perfect or as I expected and took longer than anticipated this is a decent progression point.

ToDo: Add details and specifics - Even just refer to New Folder [[Perlin Noise Application]]. 

Plan of Action - Update as Progression Flows:

1.  Add ChunkManager; We need to begin making it procedural not just entirely PerlinNoise or Hash Randomization, they are valid, but they arent a Process, hence not Procedural.
2.  When working on ChunkManager re-organize and refactor how Seeds are applied and called; Don't want seeds to be recalled constantly when they arent needed to be remade on each generation(issue for perlin noise only really).
3. Begin on Player setup; Inventory addition. - General
4. Construct Resources and ResourceAcquisition. - General
5. The meat; The Automation Sequencing and processes - Start AT LATEST, March 30th. As this will have its own spiral of content.

Bonus 1.  Add one more Noise Type; FBM(Fractal Brownian Motion). 



##### Note 5: Written on 19/3/26

This note took a little longer than expected - took 3 days to iron out this commit - i can't in good faith push busted content.

In this addition I went back and refactored and cleared up some things - I have more to refactor - nothing major just cohesive code structuring nothing from the ground up. Some elements from the World Generation and the flow of setting up the world, e.g. Noise Generators were not used with the interface due to different tasks being performed under different rules - hence mmultiple were needed, not swapped out. 
This led to just having them explicitly be called and leaving in the interfaces for my post project work on this. 

I added Elevation attribute to the tiles and this is reflected in the Tiles darkening or lightening based on their elevation in their associated biome, makes them pop differently. 

There is so much here and it is late, this will be one of the few times im going to cut to the chase and say - A LOT changed.. But a healthy portion was done.

Update 1: Added in Elevation movement restrictions to the player, done some fine tuning and have made noted plans for after the project, currently feedback is simple and rudimentary - changes player square colour to be a different colour to show that movement stopping and the colour change are significant.
ToDo: Add in tile visibility of this process, currently this is sufficient, but the larger issue here it is hard to know where to go after realizing your path is blocked by the elevation. Small but important, this is something for the post project or refinement stage because of time restrictions and this being not a large element of the project specifications and larger plan, just a very inconvenient player awareness issue.

Plan of Action - Update as Progression Flows:
Yes cleared off the whole thing while refactoring other elements - i have neglected documentation due to working.

1. Begin on Player setup; Inventory addition 2 hour~ job
2. Construct Resources and ResourceAcquisition. - General. more nuanced, this will need implementation and alterations to Tiles or rather additions, so will expand on that and will take either 3-4~ hours or a day or 2, this one has me a little strapped for time, but the actual logic implementation will be easy due to our refactors today! We can just slot it into a tile assignment.
3. The meat; The Automation Sequencing and processes - Start AT LATEST, March 30th. As this will have its own spiral of content.


##### Note 6: Written on 20/3/26

Began adding in basics of the systems, resources are assigned as a struct to Tiles and  resources can be added to inventories and there has been setup for passing between inventories for either later on when adding machines and automation or inventory pop ups to let players move 1 item set from inventory A to Inventory B.

There is no Visual Inventory UI it is just remembered in the Background - This is something i want to fix and make clearer. Inventories can be different sizes too!

Began work on resources as mentioned and they can be applied to  tiles now, but i need to implement the mining and acquiring of these resources. 

Plan of Action - Update as Progression Flows:
Yes cleared off the whole thing while refactoring other elements - i have neglected documentation due to working.

1. Begin on Player setup; Inventory addition - Added : MUST MAKE VISUAL AND INTERACTABLE.
2. Construct Resources and ResourceAcquisition. - General. more nuanced, this will need implementation and alterations to Tiles or rather additions, so will expand on that and will take either 3-4~ hours or a day or 2, this one has me a little strapped for time, but the actual logic implementation will be easy due to our refactors today! We can just slot it into a tile assignment.
   Added Resources : Must implement Resource Acquisitions.
3. The meat; The Automation Sequencing and processes - Start AT LATEST, March 30th. As this will have its own spiral of content.


##### Note 7: Written on 21/3/26

Minor update, adding mining and inventory additions - need to add in the visual components to this but i am taking this note as there is something i have noticed and want to refactor properly at a later date because it recursively appearing due to the nature of separating concerns.

Adding a Sync feature to chunkManager, currently the majority of the tile influences is done through chunk manager as we delegate it to who runs the management; but TileMap owns the Tiles and retains their info and state;
Process is; 
Setup->ChunkManager->Influences Tiles->Tiles reflect this update based on information given as they are just structs-> Updates NOT reflected in TileMap management natively, must be explicitly linked->Updated data is correct and added but not matching

Refactor is simple, have a realistic but encompassing function that will link these 2 so that the Chunks have rules that Tiles represent;
Without conflicting with the TileMap management.

TLDR;
Chunks Manage chunks and biomes and influence Tiles based on rules
TileMap manages tiles

Issue; Influences are not always translated without explicitly ensuring so, minor hiccups occur - FIX.


Plan of Action - Update as Progression Flows:
Yes cleared off the whole thing while refactoring other elements - i have neglected documentation due to working.

{
1. Begin on Player setup; Inventory addition - Basic Visuals present, want to overhaul post project
2. Construct Resources and ResourceAcquisition. - Added : But must display prompt to act and reactive messaging - e.g. Inventory full.
} -> Gracefully basically done but just hard to showcase IN-GAME, more backend. 
3. The meat; The Automation Sequencing and processes - Start AT LATEST, March 30th. As this will have its own spiral of content.


##### Note 8: Written on 22/3/26

Very mild update, refactored some things that bothered me;
Const_cast on Perlin Noise generation replaced by a simple seed cache matching the set seed check, not massive but annoyed me.

Bigger one, moved a lot of redundancies of Chunkmanager intialize into functions - this was to reduce initializes clutter. Because it was bad before hand. no initialize of a project this size should be 160 lines long... 

Forgot to update yesterdays message but got a rudimentary inventory and resource acquisiton system set up now, not marked off fully as it is architecturally not positioned great and i'd like to fix that, but for the moment it is servicable and will work but will be changed - Next steps are to begin working on the Machines and Automation processes.



##### Note 8: Written on 12/4/26

The note has been written today; 
But this is work over the past 2 and a half weeks. 


1. Added in the The Automation Basics - Constructed Registry for Machines. Machines are all data.
2. Machines consist of MachineComponents with make up their data definition, their local variables and state that are used to update them and perform actions.
3. A Tile Accessor that was used to bridge the gap betyween ChunkManager and TileMap syncing in other classes. Could make clearer in retrospect.
4. Added Manager/System for machines that updates and ticks all active machines in the world. Would like to update to be better and more ordered - currently designed as an unordered update system and is fine, but not something im super proud of - inner perfectionist is upset but as a proof of concept it works.
5. Placement Logic, A new function that is designed to permit or deny a world interaction from the player with visual feedback based on whether we permit the action the player is trying to do.

This is the cliffnotes version..