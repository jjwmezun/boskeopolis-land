# Boskeopolis Land

A dumb li'l platformer game made with C++ & SDL2.

You play as Autumn Springer & go round levels collecting gems, diamonds, & accomplishing goals. Most levels just require you to get the striped Keycane @ the end, while others have zanier goals, like collecting a certain # o' PTS from gems or racing opponents to a goal.

For some reason, I chose to make this game have a monochrome palette system. Each "GameState" has a palette which is used to paint all the textures by indexed color.

Important note 'bout image resources: all images must use the same 7-color indexed palette. Non-indexed graphics will throw an exception when loaded, as it tries to apply the palette to indexed colors & can't 'cause they're not there.

Level, map, palette, & tileset data is held in JSON files in the resources folder. Maps can have any filename & are linked from level files, but level files & block files must have a 3-digit # for a name & are loaded automatically. To add a level, just create a valid JSON file in the levels folder with a 3-digit # below the current limit specified in the Levels class ( now 64 ).

## How to Use

### Compilation

* SDL2 with SDL_image
* C++ 11 or higher
* RapidJSON with OStreamWrapper

As mentioned earlier, if you want to add images, you have to use a certain 7-color palette for all files. A GIMP palette file is provided in the resources folder. Just import this into GIMP & turn any image you want to use to indexed ( Image -> Mode -> Indexed ) & select "Use Custom Palette" & the imported palette. Make sure "Remove unused colors from colormap" us unchecked.

Editing or creating new map files requires a recent version o' Tiled ( must allow for mo' custom property values than just string ). Maps should be saved as uncompressed JSON files.

## Authors

* **J. J. W. Mezun**

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE.md](LICENSE.md) file for details.

## Dumb Things I Gotta Do Someday

### Basic Code
* Fix glitch wherein Autumn can't move upward on a ladder after a map transition.
* Sprite class still a mess -- look into organizing states ( isJumping, isDucking, onLadder, etc. ) into finite state machine ).
* Timers kinda wonky. In particular, I want to find a way to encapsulate common "if hit; else if on, update; else, start" pattern in the class itself so I don't have to keep typing it. Sometimes the stopping mechanism for TimerSimple is wonky ( see SewerMonster class ).

### Levels to Do Soon
* Petrol Pond Place
* Donut Dungeon
* Foul Fowl Farm
* Stop & Go Space Station

## Credits

### Music

All music is royalty-free music by Kevin MacLeod @ incompetech.com, all o' which is under the Creative Commons License 3.0, Attribution - http://creativecommons.org/licenses/by/3.0/.

Maybe temporary, maybe not.

* Title: Shaving Mirror
* Overworld: Breakdown
* Level Select: Too Cool
* Success: Happy Happy Game Show
* Failure: Lobby Time
* City1, City4: District Four
* Woods1, Woods2: Twisted
* Woods3: Nightmare Machine
* Mines1, Mines3, Mines4: Chillin Hard
* Desert1: Wepa
* Desert2: Mischief Maker, Clash Defiant
* Desert4: The Show Must Be Go
* Mountain1: Groundwork
* Sky1, Sky2, Sky3, Sky4: Nowhere Land
* Space1: Disco Lounge
* Ice1: Nouvelle Noel
* Ice2: Crinoline Dreams
* Pirate1, Pirate3: Salty Ditty
* Pirate2: Funin and Sunin
* Sewers1, Sewers2, Sewers3: RetroFuture Nasty
* Factory1: Robozo
* Factory3: Noise Attack
* Factory4: Corruption
* Palace1: Obliteration
* Palace3: Kool Kats
* Mart: Got Funk
* Maze: Reformat

The 1 exception is "Play in the Background"'s music, which a rendition o' Mozart's "Rondo alla turca" from Musopen @ https://musopen.org/music/2079/wolfgang-amadeus-mozart/rondo-alla-turca-from-sonata-kv-331/.