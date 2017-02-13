# Boskeopolis Land

A dumb li'l platformer game made with C++ & SDL2.

You play as Autumn Springer & go round levels collecting gems, diamonds, & accomplishing goals. Most levels just require you to get the striped Keycane @ the end, while others have zanier goals, like collecting a certain # o' PTS from gems or racing opponents to a goal.

For some reason, I chose to make this game have a monochrome palette system. Each "GameState" has a palette which is used to paint all the textures by indexed color. Maps JSON files hold colors, with code to change the LevelState's palette 'pon starting a level & changing maps.

Important note 'bout image resources: all images must use the same 7-color indexed palette. Non-indexed graphics will throw an exception when loaded, as it tries to apply the palette to indexed colors & can't 'cause they're not there.

## How to Use

### Running

This only comes with a Linux bin, & I doubt that'll work in all Linux OSes, either.

Any others will require compilation.

### Compilation

Unfortunately, as o' now, you need the latest version o' Code::Blocks to compile straight 'way from the project file. Otherwise, you'll have to create a new project in 'nother program & import the files or create a makefile.

I'm planning to make a makefile once I figure out all the kinks.

Also required:

```
SDL2 with SDL_image
C++ 11 or higher
RapidJSON with OStreamWrapper
```

As mentioned earlier, if you want to add images, you have to use a certain 7-color palette for all files. A GIMP palette file is provided in the resources folder. Just import this into GIMP & turn any image you want to use to indexed ( Image -> Mode -> Indexed ) & select "Use Custom Palette" & the imported palette. Make sure "Remove unused colors from colormap" us unchecked.

Editing or creating new map files requires a recent version o' Tiled (must allow for mo' custom property values than just string). Maps should be saved as uncompressed JSON files.

## Authors

* **J. J. W. Mezun**

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details