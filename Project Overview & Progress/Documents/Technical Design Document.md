
This will be just a overarching technical design document as I want to cover what im actually going to try implement.



***Interconnected Systems***
--
-- Mining System example -- 
An Auto Mining Device does an action, overtime its Output will fill up
-
Should the output not have anywhere to go once it is full the machine will stop its action and let the Player harvest it.
-
If it has a connected output - e.g. a valid transporter such as a conveyor, it will begin depositing it periodically on a delay - this delay could be based on a few things, could be the conveyor, it could be the Mining Machine, it could be the resource, possibilities are open and thats fun. 
-
Conveyor will update to move along the item, this will go back to the base and be dropped off the conveyor onto the floor or if there is an Input, such as a box or crate or the Home Base Rift Box(uncapped storage name pending) will deposit it, if the base box, will auto update the system, Other boxes are up in the air, should storage be allowed? or should they automatically filter into the main base box and become attached, this is something I'd like to mull over and experiment on when the time comes.
- Inventories will be added, just not crafting system I'd like to flesh it out
Base inventory is tied to the Crafting System - crafting will use those component quantities to allow you to requisition gear - blueprints and certain amounts of useful or high end gear should you be able to afford the material cost of the resources needed to acquire them.
- ==(#**- Pushed back to post project work -**#)==
Crafting System will be small in grand scheme, but will open up further progression to explore and manage this worlds resources into a tangible form you can utilize.


***Visualizer Component Animation*** ==(#**- Pushed back to post project work -**#)==
But during this process there will be a ***visualizer*** to show the machine is working - Technical addition would be updating sprite based on the input, could use a rudimentary 2D animation rig system where its components and the component is always the Transporter it has a type and a state but if given an active input will add a small addition to the conveyor and use its item lifetime as a method to see how far on the conveyor line it is and it will know when to stop displaying - using the item for moving.

Addendum:
Player UI I would like to be diagetic, this would mean I'd need player model to update, if I can get this component system working, i could make a similar or unique version of the players where items held will rig the player to animate certain ways. This may be larger than im scoping it as, but that is what the research period will be for! Worst case scenario i default to hardcoded sprites to represent changes, slower and more tedious, but, simpler.

NOTE: Items will be a large system, so it will be scrapped or pushed till post project efforts - e.g. Pickaxe and variations, Structure Items such as Scaffolding or a pulley system to traverse high terrain without upgrades - which also will be scrapped for time..