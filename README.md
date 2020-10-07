# Boskeopolis Land

## ¿What Is this Game?

A simple platformer based on the short-story series, (_Boskeopolis Stories_)[https://boskeopolis-stories.com/].

You play as thief Autumn Springer in her rhymin’ & stealin’ ventures round Boskeopolis, collecting gems, diamonds, & accomplishing each level’s goal to continue on to the next level. Most levels just require you to get the red-&-white-striped delectable “Keycane” @ the end, while others have zanier goals, like collecting a certain # o’ ₧ ( Boskeopoleon currency ) from gems, collecting a key & reaching a treasure chest, or surviving for a certain amount o’ time. These goals are shouted @ you @ the start o’ every level like a _WarioWare_ minigame.

## ’Bout the Development o’ this Game

For some reason, I chose to make this game have a monochrome palette system. Each “GameState” has a palette which is used to paint all the textures by indexed color.

Important note ’bout image resources: all images must use the same 7-color indexed palette. Non-indexed graphics will throw an exception when loaded, as it tries to apply the palette to indexed colors & can’t ’cause they’re not there.

Level, map, palette, & tileset data is held in JSON files in the resources folder. Maps can have any filename & are linked from level files, but level files & block files must have a 3-digit # for a name & are loaded automatically. To add a level, just create a valid JSON file in the levels folder with a 3-digit # below the current limit specified in the Levels class ( now 64 ).

## How to Use

### Compilation

* SDL2 with SDL_image
* C++ 11 or higher
* RapidJSON with OStreamWrapper

As mentioned earlier, if you want to add images, you have to use a certain 7-color palette for all files. A GIMP palette file is provided in the resources folder. Just import this into GIMP & turn any image you want to use to indexed ( Image -> Mode -> Indexed ) & select “Use Custom Palette” & the imported palette. Make sure “Remove unused colors from colormap” us unchecked.

Editing or creating new map files requires a recent version o’ Tiled ( must allow for mo’ custom property values than just string ). Maps should be saved as uncompressed JSON files.

## Authors

* **J. J. W. Mezun**

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE.md](LICENSE.md) file for details.

All artwork, including graphics & text, are released under Creative Commons Attribution-ShareAlike 4.0 International ( https://creativecommons.org/licenses/by-sa/4.0/ )

## Credits

### Music

The vast majority o’ music used is royalty-free music by Kevin MacLeod @ incompetech.com, all o’ which is under the Creative Commons License 3.0, Attribution — http://creativecommons.org/licenses/by/3.0/.

* Title: Shaving Mirror
* Overworld: Breakdown
* Level Select: Too Cool
* Success: Happy Happy Game Show
* Failure: Lobby Time
* City1, City2, City4: District Four
* City3: Nouvelle Noel
* Domestic1: Le Grand Chase
* Domestic2: Leaving Home
* Woods1, Woods2: Breakdown
* Woods3: Nightmare Machine
* Woods4: Autumn Day
* Mines1, Mines3, Mines4: Secrets of the Schoolyard
* Desert1: Wepa
* Desert2: Mischief Maker, Clash Defiant
* Desert3: DD Groove
* Desert4: The Show Must Be Go
* Mountain1 & Mountain2: Groundwork
* Sky1, Sky2, Sky3: Nowhere Land
* Sky4: Dreamer
* Space1: Disco Lounge
* Space3: Local Forecast – Elevator
* Space4: Beauty Flow
* Ice1 & Ice2: Crinoline Dreams
* Ice3 & Ice4: Chillin Hard
* Pirate1: Salty Ditty
* Pirate2, Swamp3: Rocket Power
* Swamp1, Swamp2, Swamp4: Netherworld Shanty
* Sewers1, Sewers2, Sewers3: RetroFuture Nasty
* Sewers4: Raw
* Factory1: Robozo
* Factory3: Noise Attack
* Factory4: Furious Freak
* Palace1: Obliteration
* Palace3: Kool Kats
* Palace4: Take the Lead
* Mart: Got Funk
* Maze: Reformat

Other songs used:

* “Play in the Background”: a rendition o’ Mozart’s “Rondo alla turca” from Musopen @ https://musopen.org/music/2079/wolfgang-amadeus-mozart/rondo-alla-turca-from-sonata-kv-331/.
* “Petrol Pond Place” & “Good Shop Lifestyle”: “Skyhawk Beach” by Blue Wave Theory @ http://freemusicarchive.org/music/Blue_Wave_Theory/Surf_Music_Month_Challenge/Skyhawk_Beach_fade_in. Released under Creative Commons Attribution-ShareAlike 4.0 International ( https://creativecommons.org/licenses/by-sa/4.0/ ).
* “Mind Your Manors”: “Arround the Village” by Lobo Loco @ https://freemusicarchive.org/music/Lobo_Loco/Good_Old_Organ_Ice/Arround_the_Village_ID_1183. Released under Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International ( https://creativecommons.org/licenses/by-nc-nd/4.0/ ).
* “Castle Chamsby”, 1st Outside Area: “Soothing Nighttime Ambience” by SPANAC @ https://www.freesoundslibrary.com/soothing-nighttime-ambience/. Released under Attribution 4.0 International ( https://creativecommons.org/licenses/by/4.0/ ).
