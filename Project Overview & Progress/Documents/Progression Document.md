 
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