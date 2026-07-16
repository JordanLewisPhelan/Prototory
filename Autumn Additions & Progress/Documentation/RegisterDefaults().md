### *<u>Usage & Setting Up Variants</u>*

  **Subject to Slight Changes**

Pre-Programming suggestion!
For best results evenly space your sprites and keep their size consistent, good practise and will make using this a little easier!

#### **Notice:**
The current setup and examples are hardcoded, I have not yet added a JSON parser or a similar tool to make it so you can add data into a central spot, register that and work from that, this is a step by step on how to utilize it, it is currently clunky but I hope to amend that before this project is put back on the shelf.

1. Add your sprite sheet to the project, remember where you put it; The Project standard is currently in "ASSETS\\IMAGES\\-YourSpriteSheet-".
2. Make a string and assign the string the FilePath in the project, tell it where to go to get your Sprites
3. Tell { m_typeToTexturePaths } what TileType you are attempting to reach - it uses basic integers, don't forget to static_cast TileType to ensure its readable.
   Then assign your string path to it, that TileType will now use your spriteSheet.
4. Next we want to use GridLayout class to understand how we should cut up your sprite sheet to extract the Sprites and ideally leave no coarse edges or taking too much.
   
   - 4.1 Understand how many Rows and Columns your sprite sheet has and input those Values in the first 2 parameters for GridLayout, it is Rows and Columns respectively, important if you have different size rows and columns.
     
   - 4.2 If possible to prevent extra testing check the size of your sprites in pixels, if you used Aseprite to draw your sprites you can use it to figure out the dimensions, then use those dimensions in an sf::Vector2i(). This is your 3rd parameter.
     
  - 4.3 Make another Vector2i() - This 4th parameter will be the spacing between your sprites, as a result recommended that spacing be consistent in sprite sheet.
    
  - 4.4 The 5th parameter is only used one but important for anyone who does not have their sprites edge to edge of their sprite sheet. This Vector2i() will be how far from the Left of the image | how far from the Top of the image to start the cutting process. In my sprites the first sprite is indented, so I use this to react the Top-Left corner of the sprite as that is how SFML handles images.
5. The final step is one I will join together as you can copy the example already in RegisterDefaults, I do not want to type out each step here... But this is where we use step 4s result, a usable GridLayout to segment your sprite sheet. We then know how many sprites we are expecting. 
   We will just make an sf::IntRect for each sprite as that will give those sprites their dimensions. 
   Then we add our sf::IntRects we are making to the { m_variants } list, make sure you tell it what TileType it is going into and then use .push_back(TileVariant { -YourIntRect- });
   This will add all of the rects we segmented from the sliceIntoRects function and put them into the chosen tiles available variants, and that will automatically work on any world generated now!
### *<u>Description</u>*
This is a very important feature as it ensures we have SpriteSheets or images to reference and segment into usable sprites.

The idea is simple, you add in a filePath, wherever you have your Image sheet - ***Usability Note: Could probably adjust to be a list that this calls from and assigns automatically and you add to that list.***

Then load that texture and assign the filepath to the list of Texture-TileTypes unordered map variable named { m_typeToTexturePaths } in the game. This ties that filePath to the Type so whenever you want to get valid variants of that type of Tile, you can just look it up!
This streamlines the process so that all bound sprite sheets. 

---

### **Notice:** 
That was the plan but in my *infinite wisdom*, I forgot to double check that and it has now dawned on me I have not correctly set that up, the current way this is set up I was testing both worked and forgot that the path to the texture is overwritten.
e.g. TileType::Grass gets its sprites, then we get to rock and we try to allow rocks and grass to spawn in the same biome. But we are overwriting the path, so the old rect dimensions now map to different parts of of the Rock sprites.

So it does work, can add other rect dimensions but because the spriteSheet is overwitten in the current setup it does not persist the Grass Sprites. 

The fix is simple but It is late, will add it in the next day I am working and then mark this resolved - I feel I should leave this in as a reminder to myself. Even something thought through and functioning.. might not function how I expect. 