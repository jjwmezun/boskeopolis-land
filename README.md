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

* A rendition o’ Mozart’s “Rondo alla turca” from Musopen
  Link: https://musopen.org/music/2079/wolfgang-amadeus-mozart/rondo-alla-turca-from-sonata-kv-331/
  License: Public Domain

* “Arround the Village” by Lobo Loco
  Link: https://freemusicarchive.org/music/Lobo_Loco/Good_Old_Organ_Ice/Arround_the_Village_ID_1183
  License: https://creativecommons.org/licenses/by-nc-nd/4.0/

* Autumn Day by Kevin MacLeod
  Link: https://filmmusic.io/song/3394-autumn-day
  License: https://filmmusic.io/standard-license

* Beauty Flow by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/5025-beauty-flow
  License: https://filmmusic.io/standard-license

* Breakdown by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/3456-breakdown
  License: https://filmmusic.io/standard-license

* Chillin Hard by Kevin MacLeod
  Link: https://incompetech.com/wordpress/2016/06/chillin-hard/
  License: https://filmmusic.io/standard-license

* Clash Defiant by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/3510-clash-defiant
  License: https://filmmusic.io/standard-license

* Crinoline Dreams by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/3560-crinoline-dreams
  License: https://filmmusic.io/standard-license

* Dispersion Relation by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/3657-dispersion-relation
  License: https://filmmusic.io/standard-license

* District Four by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/3662-district-four
  License: https://filmmusic.io/standard-license

* DD Groove by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/3578-dd-groove
  License: https://filmmusic.io/standard-license

* Disco Lounge by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/3651-disco-lounge
  License: https://filmmusic.io/standard-license

* Dreamer by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/3676-dreamer
  License: https://filmmusic.io/standard-license

* Got Funk by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/3819-got-funk
  License: https://filmmusic.io/standard-license

* Groundwork by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/3832-groundwork
  License: https://filmmusic.io/standard-license

* Happy Happy Game Show by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/3856-happy-happy-game-show
  License: https://filmmusic.io/standard-license

* Kool Kats by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/3956-kool-kats
  License: https://filmmusic.io/standard-license

* Leaving Home by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4708-leaving-home
  License: https://filmmusic.io/standard-license

* Le Grand Chase by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4670-le-grand-chase
  License: https://filmmusic.io/standard-license

* Lobby Time by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/3986-lobby-time
  License: https://filmmusic.io/standard-license

* Local Forecast - Slower by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/3988-local-forecast---slower
  License: https://filmmusic.io/standard-license

* Mischief Maker by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4059-mischief-maker
  License: https://filmmusic.io/standard-license

* Netherworld Shanty by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4119-netherworld-shanty
  License: https://filmmusic.io/standard-license

* Nightmare Machine by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4133-nightmare-machine
  License: https://filmmusic.io/standard-license

* Noise Attack by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4142-noise-attack
  License: https://filmmusic.io/standard-license

* Nouvelle Noel by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4147-nouvelle-noel
  License: https://filmmusic.io/standard-license

* Nowhere Land by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4148-nowhere-land
  License: https://filmmusic.io/standard-license

* Obliteration by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4151-obliteration
  License: https://filmmusic.io/standard-license

* Raw by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4264-raw
  License: https://filmmusic.io/standard-license

* Reformat by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4269-reformat
  License: https://filmmusic.io/standard-license

* RetroFuture Nasty by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4279-retrofuture-nasty
  License: https://filmmusic.io/standard-license

* Rocket Power by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4303-rocket-power
  License: https://filmmusic.io/standard-license

* Robobozo by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4299-robobozo
  License: https://filmmusic.io/standard-license

* Salty Ditty by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4315-salty-ditty
  License: https://filmmusic.io/standard-license

* Shaving Mirror by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4344-shaving-mirror
  License: https://filmmusic.io/standard-license

* Secrets of the Schoolyard by Kevin MacLeod
  Link: https://incompetech.com/wordpress/2015/02/secrets-of-the-schoolyard/
  License: https://filmmusic.io/standard-license

* “Skyhawk Beach” by Blue Wave Theory
  Link: http://freemusicarchive.org/music/Blue_Wave_Theory/Surf_Music_Month_Challenge/Skyhawk_Beach_fade_in
  License: https://creativecommons.org/licenses/by-sa/4.0/

* “Soothing Nighttime Ambience” by SPANAC
  Link: https://www.freesoundslibrary.com/soothing-nighttime-ambience/
  License: https://creativecommons.org/licenses/by/4.0/

* Take the Lead by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4458-take-the-lead
  License: https://filmmusic.io/standard-license

* The Show Must Be Go by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4509-the-show-must-be-go
  License: https://filmmusic.io/standard-license

* Too Cool by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4534-too-cool
  License: https://filmmusic.io/standard-license

* Wepa by Kevin MacLeod
  Link: https://incompetech.filmmusic.io/song/4615-wepa
  License: https://filmmusic.io/standard-license