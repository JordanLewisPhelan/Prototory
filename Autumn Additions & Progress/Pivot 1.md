Moving away from Visual Constructor idea into Visuals.

This seems unworthy of note, but this changes things from being a central manager into being;

1. UIVisuals or InventoryVisual
2. TileVisuals 

This follows up as 2 very independent topics, Inventory and Tile would still maintain the flow I wanted for visuals but it is now specified and more deliberate. 

Also no confusion when seeing a Visual child as you know what is expected to be there which will be better for readability in the long run.

I think my VisualConstructor idea does have legs, but I think what I was trying to assign it to do just doesn't actually make sense trying to force it to fit all purposes by itself; VisualManager may have a place down the line actually.

I could use that to compile information and sync it to the tile map in 1 pass. 
This is thinking and typing, will play it by feel for now and focus on Tile and UI/Inventory visuals.