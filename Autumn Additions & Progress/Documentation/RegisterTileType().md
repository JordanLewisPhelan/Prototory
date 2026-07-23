### *<u>Params & Return Values</u>*

**Returns:**  void

**Params:** Names as listed in definition
- TileType : t_type 
  Is the TileType we are looking to have fill in, used in function to call GetTextureForType() - which simply uses available TileTypes and a linked filePath to join a Type to a sprite sheet.
- const std::string& t_filePath
  A read only reference to the file path where our sprite image is located.
- const GridLayout& t_layout
  Read only reference to a GridLayout parameters made elsewhere, These will dictate how we turn an Image into sprites.
	  - Rows and columns of sprites; Basically saying how many sprites we are expecting from this, e.g. Rows = 3, Columns = 4, We will end up with 12 sprites.
	  - Vector2i of how big the expected sprite should be, e.g. 64x64 pixels, we can get a perfect shape and size of the image.
	  - Vector2i that determines how far apart each image is apart e.g. 8x16, this willl mean the spacing from sprite 1 and 2 on the x axis is 8 px, but then sprite 1 and 2 on the y axis is 16 pixels apart. 
	    Ensures specificity and safety when working with sprite sheets as some form of consistency is always recommended, just input the dimensions and it will give out the expected sprites.
	  - Vector2i - This last one is only an initial offset; Usually sprite sheets do not start in the top left corner pixel to pixel of a full image, this lets us indent where we start the process, e.g. 16x16 : 16 pixels right from the left, 16 pixels down from the top of the image before the sprites start.

### *<u>Description</u>*
This function is called by ([[RegisterDefaults()]]), the Idea is tying the setup and registration to the TileTypes.

Once we enter this function we do 1 simple check, Does the filePath we got when this was called resolve? 
If yes, continue, if no, We stop early.

Then we use our list of valid loaded textures to see if it has been loaded - it will fail if not loaded ahead of time. 

But we really only need the texture here and what the expected TileType is, this is simply linking a TileType and its TileVariant, i.e. when calling TileType::Grass, we can get a grass variant, if we assign TileType::Grass here that will make whatever texture we are registering here become a grass variant during generation.

```c++
	for (const sf::IntRect& l_rect : sliceIntoRects(t_layout))
	{
		m_variants[static_cast<int>(t_type)].push_back(TileVariant{ l_rect, l_texture });
	}
```
This for loop uses the IntRect dimensions given in 't_layout' and converts them into smaller images, that is what is shoved into the TileType value in m_variants, this segments TileType::Grass from TileType::Sand.
The reason we push both the l_rect and l_texture is because we need;
- The dimensions on the sprite sheet recallable as it will be built later
- The texture referenceable, storing it here ensures whenever we do build it later the texture still exists and hasn't been overwritten.
