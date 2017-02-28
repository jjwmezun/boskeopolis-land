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

* SDL2 with SDL_image
* C++ 11 or higher
* RapidJSON with OStreamWrapper

As mentioned earlier, if you want to add images, you have to use a certain 7-color palette for all files. A GIMP palette file is provided in the resources folder. Just import this into GIMP & turn any image you want to use to indexed ( Image -> Mode -> Indexed ) & select "Use Custom Palette" & the imported palette. Make sure "Remove unused colors from colormap" us unchecked.

Editing or creating new map files requires a recent version o' Tiled (must allow for mo' custom property values than just string). Maps should be saved as uncompressed JSON files.

## Authors

* **J. J. W. Mezun**

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE.md](LICENSE.md) file for details.

## Dumb Things I Gotta Do Someday

### Basic Code Organization
* Organize tileset data (Perhaps separate into different tilesets for different maps).
* Organize graphics (should probably separate sprites, backgrounds, tilesets, & such.

### Levels
* Value Valhalla
	* Polish level design.
	* Include diamond.
* Owl Level
	* Finish map.
	* Draw owl.
* Soupy Sewers
	* Fix buggy, crappy ending o' sewer map.
	* Finish sewer-monster graphics.
	* Improve fish graphics.
	* Animate water.
	* Maybe darken BG.
* Hot Shop
	* Fix bug where cart gets stuck inside ground on mountain just before long drop.
	* Add graphics for player & enemy carts.
	* Improve spring graphics.
	* Fix cloudy background so shelves don't appear 'hind it.
* Minty Mines
	* Figure out what I want to do with this level.
* Mustard Mountain
	* Figure out what I want to do with this level (probably make it focus on arial movement).
	* Add mo' slope types.
* Snowy Slopes
	* Figure out what I want to do with this level.
* Steam Engeenius
	* Make map.
* The Streets o' Storms
	* Make map.
* Curse o' the Ladder-Splayed Caves
	* Figure out what else to do with this level.
	* Fix level-select screen so this level name doesn't splay out into the scores (should make level names take up a'least 2 miniblocks o' vertical space & have them have a line-limit set).
* Dry Drought Desert
	* Think o' something to do with this level.
* Pepperoncini Pyramid
	* Make a map.
* Level Industries
	* Figure out what I want to do with this level.
* Windy Woods
	* Think o' something to do with this level.
* The Minus Touch
	* Think o' something to do with this level.
* Sawdust Crush
	* Fix glitch where you get bonked from 'bove in certain places.
	* Improve saw graphics & maybe make a different BG.