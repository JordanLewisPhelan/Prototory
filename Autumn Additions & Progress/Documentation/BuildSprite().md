### *<u>Params & Return Values</u>*

**Returns:**  sf::Sprite or std::nullopt
- nullopt is returned if the sprite dimensions are not valid primarily.
- sf::Sprite is returned when we determine the variant sprite we are passing back, in this case its a subsection of the TileType sprite sheet that is assigned and registered in ([[RegisterDefaults()]]).

**Params:** Names as listed in definition
- TileType : t_type 
  Is the TileType we are looking to have fill in, used in function to call GetTextureForType() - which simply uses available TileTypes and a linked filePath to join a Type to a sprite sheet.
- int : t_variantIndex
  Used alongside t_type to determine what variant we want from the list.
- int : t_rotationStep
  Tells the sprite how many times to rotate 90 degrees. This is basically just orienting the sprite so that 1 sprite sheet variant has more flavour and more variation.
  Critical to recall this is not the Angle we want the sprite to be at, this is the rotation increment; e.g. 0 == 0 degrees, 1 == 90 degrees, 2 == 180 and 3 == 270 degrees.
  
### *<u>Description</u>*

The simplest description is we have sprites made and assigned from ([[RegisterDefaults()]]), so now we just want to create those sprites. 

This allows developers or modders to simply add in their own sprites following same conventions and it will randomize variants and rotation for any sprite list added. Also ensures that if new sprites are added to a tiletype that does exist already, it simply adds it to the existing list, i.e. multiple sprite sheets can be used to extend the Registry easily.

In this project the use case is a HashNoise number is used to say, this tile returns variant 4 of a sprite sheet consisting of 9 sprites. 
We take the variants registered into m_variants(which without getting into is just the TileTypes enum int, with a list of tileVariants that have their sprite square outlined, e.g. 64, 64 pixels - that would capture the test tiles space - Note maybe worth a deeper explanation? is a crucial member variable..) and ask for the tileVariant at t_variantIndex, this gives us that variant sprite dimensions which is used to cut out that piece from the sprite sheet and use it as the sprite. 



- Note: Potentially overexplaining or underexplaining, may need outside help to see if they understand this explanation or if I should be more explicit or refrain from mentioning buildSprite inner machinations all here. 

