As I was adding in and testing the sprites and moving between them at different scales - Some sheets I found only, namely RockTest.png had more rows and columns, but I wanted to test these. I reshaped them in Aseprite but didn't realize until I got into testing that the sizes between sprite became malformed, but they still took up the right space, they just ended up leaning into some sprites were cut off and joined up with the next.

My idea is obviously to just enforce a more stable step between sprites but it made me conjure up this simple idea.


What if I could Layer Sprites on 1 tile? The rough idea is tiles remain as place mats, fail-safe sprites and positional awareness, the sprite sit over it. 
But what if I added on layered sprites where mutliple sprites can sit on 1 tile, this way there is even more procedural work and less moments where sprites don't fit tiles.

Now the sad truth is this idea came from the simple fact the online sheet has a different size and even a full sprite image has holes in it for texture so it wouldn't fill in the whole tile. This wouldn't have been an idea had I;

1. Not skimmed for an easy sprite sheet.
2. Realized the space sizing error.
3. The sprite sheet itself had natural gaps.

This idea has holes of course, I'd have to design stuff wrong intentionally with gaps and hope random generation does not in rare instances rotate 2 sprites perfectly to show a gap. 
Remake the sprite sheets with that idea and intentionally allow the algorithm to capture sprite images be wrong. 

Which is why its here. Its not out of consideration, but it would require a different kind of change and angle to actively set up versus getting this done now and adding it on later.

Below is an example that made me conjure the idea, I placed the built sprites next to player for viewing, and realized the gap was present, solution was to fix spacing but the idea here was a follow up that i thought would be very cool.
![[brokenSpriteExample.png]]