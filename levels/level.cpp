




// Name
//===================================
//
// Level
//


// DEPENDENCIES
//===================================

    #include "avoid_money_goal.h"
    #include "map_layer_constellation.h"
    #include "map_layer_image.h"
    #include "map_layer_water.h"
    #include "camera.h"
	#include <cassert>
    #include "collect_goal.h"
    #include "do_nothing_goal.h"
    #include "game_state.h"
    #include "level.h"
    #include "sprite.h"
    #include "sprite_system.h"
    #include "starving_goal.h"
    #include "survive_time_goal.h"
    #include "timed_goal.h"
    #include "windy_goal.h"


// STATIC PROPERTIES
//===================================

// METHODS
//===================================

    Level Level::makeLevel( LevelName lvname )
    {
        switch( lvname )
        {
            case ( LV_SNEAK ):
                return
                {
                    lvname,
                    {
                        std::move( Map::mapFromPath
                        (
                            "sneak-1",
                            { Palette::PaletteType::SHADOW, 7 },
                            {
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_MOON, 32, 32, 240, 32, 0, 0, 1, false, false ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_RAIN_2, 8, 8, 0, 0, 0, 0, 16, true, true, 0, 0, 4 ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_DARKROOM, 128, 160, 0, -72, .5, .8 )
                            }
                        ) )
                    },
                    std::unique_ptr<Goal> ( new Goal( "!Don't let the guards see you!" ) ),
                    16,
                    26*16,
                    0,
                    0,
                    "Guards can see 'bout 8 blocks\n'head, 1 block 'bove if you're  on the ground, & a li'l higher\nif you're in the air.\n\nBut they can't see you if part\no' you is covered by a solid\nblock."
                };
            break;

            case ( LV_CITY_4 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "city-4",
                            { Palette::PaletteType::GRAYSCALE, 7 },
                            { new MapLayerImage( Graphics::SpriteSheet::LVBG_CITY, 264, 144, 0, 0, .5, 0 ) },
                            {},
                            { new MapLayerImage( Graphics::SpriteSheet::LVBG_RAIN, 8, 8, 0, 0, 0, 0, 2 ) }
                        )
                    },
                    std::unique_ptr<Goal> ( new Goal() ),
                    16,
                    48
                };
            break;
            case ( LV_DESERT_1 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "desert-1",
                            { Palette::PaletteType::MIDNIGHT_BLUE, 2 },
                            {
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_GRADIENT, 8, 160, 0, 0, 0, 0 ),
                            }
                        )
                    },
                    std::unique_ptr<Goal> ( new DoNothingGoal() ),
                    144,
                    90
                };
            break;


            case ( LV_DESERT_2 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "desert-2",
                            { Palette::PaletteType::MUSTARD_ORANGE, 2 },
                            {
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_GRADIENT, 8, 160, 0, 0, 0, 0 ),
                            }
                        )
                    },
                    std::unique_ptr<Goal> ( new StarvingGoal() ),
                    80,
                    14*16
                };
            break;

            case ( LV_MINES_1 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "mines-1",
                            { Palette::PaletteType::SUBTERRANEAN_TEAL, 4 }
                        )
                    },
                    std::unique_ptr<Goal> ( new Goal() ),
                    71*16,
                    19*16
                };
            break;

            case ( LV_MINES_2 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "mines-2",
                            { Palette::PaletteType::CEMENT_GRAY, 4 }
                        )
                    },
                    std::unique_ptr<Goal> ( new Goal() ),
                    3*16,
                    0
                };
            break;

            case ( LV_MINES_4 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "mines-4",
                            { Palette::PaletteType::GOLDEN_YELLOW, 4 }
                        )
                    },
                    std::unique_ptr<Goal> ( new AvoidMoneyGoal() ),
                    153*16,
                    32
                };
            break;

            case ( LV_SNOW_1 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "snow-1",
                            { Palette::PaletteType::ICY_BLUE, 0 },
                            {
                                //new MapLayerImage( Graphics::SpriteSheet::LVBG_SNOWFIRS, 256, 256, 0, 64, .25, 1, 1, true, false )
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_SNOWFIRS2, 164, 256, 0, 16, .5, .5, 1, true, false )
                            }
                        )
                    },
                    std::unique_ptr<Goal> ( new Goal() ),
                    16,
                    176
                };
            break;

            case ( LV_WOODS_2 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "woods-2",
                            { Palette::PaletteType::GOLDEN_YELLOW, 2 },
                            {
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_WOODS, 256, 256, 0, 0, .25 ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_WOODS2, 640, 256, 0, 0, .5 )
                            }
                        )
                    },
                    std::unique_ptr<Goal> ( new WindyGoal() ),
                    80,
                    27*16
                };
            break;

            case ( LV_WOODS_3 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "woods-3",
                            { Palette::PaletteType::MUSTARD_ORANGE, 2 },
                            {
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_FIRS, 461, 256, 0, -24, .1, .5 ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_WOODS, 256, 256, 0, 0, .25 ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_WOODS2, 640, 256, 0, 0, .5 )
                            }
                        )
                    },
                    std::unique_ptr<Goal> ( new Goal() ),
                    16,
                    15*16
                };
            break;

            case ( LV_SAW ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "saw-1",
                            { Palette::PaletteType::ICY_BLUE, 5 },
                            { new MapLayerImage( Graphics::SpriteSheet::LVBG_CITY, 264, 144, 0, 0, .5, 0 ) }
                        )
                    },
                    std::unique_ptr<Goal> ( new Goal( "¡Get 'way from the saw!" ) ),
                    14*16,
                    176
                };
            break;

            case ( LV_MOUNTAIN_1 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "mountain-1",
                            { Palette::PaletteType::DUSTY_BROWN, 2 },
                            { new MapLayerImage( Graphics::SpriteSheet::LVBG_MOUNTAIN_3, 256, 160, 0, 32, .5, .5, 1, true, false ) }
                        )
                    },
                    std::unique_ptr<Goal> ( new Goal() ),
                    32,
                    7*16
                };
            break;

            case ( LV_CITY_2 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "city-2",
                            { Palette::PaletteType::MIDNIGHT_RED, 7 },
                            {
                                new MapLayerConstellation( 330, 16 ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_CITY_2, 264, 160, 0, 0, .25, 0 ),
                            },
                            {},
                            { new MapLayerImage( Graphics::SpriteSheet::LVBG_RAIN, 8, 8, 0, 0, 0, 0, 16, true, true, 0, 0, 4 ) }
                        )
                    },
                    std::unique_ptr<Goal> ( new Goal( "¡Get to the Keycane before \"Dagny\"!" ) ),
                    //30*16,
                    //32*16,
                    //180*16,
                    23*16,
                    8*16,
                    //313*16,2*16,
                    //162*16,4*16,
                    Unit::BlocksToPixels( 17 )
                };
            break;

            case ( LV_WOODS_1 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "woods-1",
                            { Palette::PaletteType::CLASSIC_GREEN, 2 },
                            {
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_FIRS, 461, 256, 0, -64, .1, .25 ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_WOODS, 256, 256, 0, 0, .25 ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_WOODS2, 640, 256, 0, 0, .5 )
                            }
                        )
                    },
                    std::unique_ptr<Goal> ( new Goal() ),
                    16,
                    208
                };
            break;

            case ( LV_MAZE_1 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "maze-1",
                            { Palette::PaletteType::BABY_BLUE, 6 }
                        )
                    },
                    std::unique_ptr<Goal> ( new CollectGoal( 66900, "¡Collect everything & dodge the eyes!" ) ),
                    19*16,
                    17*16
                };
            break;

            case ( LV_FACTORY_1 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "factory-1",
                            { Palette::PaletteType::DUSTY_BROWN, 3 }
                        )
                    },
                    std::unique_ptr<Goal> ( new Goal() ),
                    16*3,
                    16*14
                };
            break;

            case ( LV_FACTORY_2 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "factory-2",
                            { Palette::PaletteType::FIERY_RED, 6 },
							{
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_FIREGLOW, 16, 32, 0, 7*16, 1, 1, 6, true, false, 0, 0, 1, true ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_GRILL, 8, 8 )
							}
                        )
                    },
                    std::unique_ptr<Goal> ( new SurviveTimeGoal( 30, "¡Last 30 seconds without being cooked!" ) ),
                    16*5,
                    -24
                };
            break;

            case ( LV_SNOW_2 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "snow-2",
                            { Palette::PaletteType::TRISTE_BLUE, 2 },
                            {
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_MINTLINES, 8, 8, 0, 0, 0, 0 )
                                //new MapLayerImage( Graphics::SpriteSheet::LVBG_CLOUDS_2, 128, 128, 0, 0, .1, .1 ),
                                //new MapLayerImage( Graphics::SpriteSheet::LVBG_CLOUDS, 128, 128, 0, 0, .25, .25 ),
                                //new MapLayerImage( Graphics::SpriteSheet::LVBG_MOUNTAIN_2, 128, 96, 0, 96, .25, .25, 1, true, false )
                            }
                        )
                    },
                    std::unique_ptr<Goal> ( new Goal() ),
                    16*1,
                    16*15
                };
            break;

            case ( LV_SKY_1 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "sky-3",
                            { Palette::PaletteType::TRISTE_BLUE, 5 },
                            {
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_CLOUDS_2, 128, 128, 0, 0, .1, .1, 1, true, true, 1 ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_CLOUDS, 128, 128, 0, 0, .25, .25, 1, true, true, -1 ),
                            }
                        )
                    },
                    std::unique_ptr<Goal> ( new Goal( "By the way, Autumn's an owl now." ) ),
                    58*16,
                    40*16
                };
            break;

            case ( LV_SKY_2 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "sky-2",
                            { Palette::PaletteType::PEARL_PINK, 3 },
                            {
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_CLOUDS_2, 128, 128, 0, 0, .1, .1, 1, true, true, 100, 50 ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_CLOUDS, 128, 128, 0, 0, .25, .25, 1, true, true, -400, -100 ),
                            }
                        )
                    },
                    std::unique_ptr<Goal> ( new CollectGoal( 15000 ) ),
                    10*16,
                    4*16
                };
            break;

            case ( LV_PYRAMID_1 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "pyramid-1",
                            { Palette::PaletteType::MUSTARD_ORANGE, 2 }
                        )
                    },
                    std::unique_ptr<Goal> ( new Goal() ),
                    80,
                    16*91
                };
            break;

            case ( LV_CART ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "cart-1",
                            { Palette::PaletteType::PEARL_PINK, 2 },
                            {

                                new MapLayerImage( Graphics::SpriteSheet::LVBG_SHOP, 37, 16 ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_SEWER_2, 320, 160, 0, 80*16, 1, 1, 1, true, false ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_SEWER_2, 320, 160, 0, 90*16, 1, 1, 1, true, false ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_CLOUDS, 128, 128, 0, 0, 1, 1, 1, true, false ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_CLOUDS, 128, 128, 0, 8*16, 1, 1, 1, true, false ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_CLOUDS, 128, 128, 0, 16*16, 1, 1, 1, true, false ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_CLOUDS, 128, 128, 0, 24*16, 1, 1, 1, true, false )
                            }
                        )
                    },
                    std::unique_ptr<Goal> ( new Goal( "¡Catch the sale on rare collectibles!" ) ),
                    -32,
                    16*/*49*/68
                };
            break;

            case ( LV_SEWER_1 ):
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "sewer-1-2",
                            { Palette::PaletteType::BABY_BLUE, 2 },
                            {
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_CLOUDS, 128, 128, 0, 0, .25, .25, 1, true, true, -50 ),
                                //new MapLayerImage( Graphics::SpriteSheet::LVBG_GRADIENT, 8, 160, 0, 0, 0, 0 ),
                                //new MapLayerConstellation( 20, 9 ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_CITY_2, 264, 160, 0, 0, .5, 0 )
                            },
                            { Warp( 1, 80, -32, 0, 20, 0, 16 ) }
                        ),
                        Map::mapFromPath
                        (
                            "sewer-1",
                            { Palette::PaletteType::SEWER_GREEN, 3 },
                            { new MapLayerImage( Graphics::SpriteSheet::LVBG_SEWER_2, 320, 160, 0, 0, .5, .5 ) },
                            { Warp( 2, 80, 160, 0, 300, 0, 64 ) },
                            { new MapLayerWater( 18 ) }
                        ),
                        Map::mapFromPath
                        (
                            "sewer-1-3",
                            { Palette::PaletteType::POISON_PURPLE, 5 },
                            {
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_GRADIENT, 8, 160, 0, 0, 0, 0 ),
                                new MapLayerConstellation( 128, 16 ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_CITY_2, 264, 160, 0, 0, .5, .25 ),
                            }
                        )
                    },
                    std::unique_ptr<Goal> ( new Goal() ),
                    16*5,
                    16*5
                };
            break;

            default:
                return
                {
                    lvname,
                    {
                        Map::mapFromPath
                        (
                            "city-1",
                            { Palette::PaletteType::MIDNIGHT_BLUE, 4 },
                            {
                                new MapLayerConstellation( 224, 16 ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_CITY_2, 264, 160, 0, 0, .5, .25 ),
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_CITY_3, 48, 32, 0, 176, 1, 1, 1, true, false )
                            },
                            { Warp( 1, 32, 32, 140, 160, 0, 16 ), Warp( 1, 17*16, 32, 170, 200, 0, 16 ) }
                        ),
                        Map::mapFromPath
                        (
                            "city-1-2",
                            { Palette::PaletteType::GOLDEN_YELLOW, 4 },
                            {
                                new MapLayerImage( Graphics::SpriteSheet::LVBG_SEWER_1, 360, 229, 0, -16, 0, 0 )
                            },
                            { Warp( 0, 153*16, 11*16, 0, 5, 0, 16 ), Warp( 0, 181*16, 11*16, 15, 20, 0, 16, 176*16 ) }
                        )
                    },
                    std::unique_ptr<Goal> ( new CollectGoal( 10000 ) ),
                    12*16,
                    11*16,
                    10 * 16,
                    0,
                    "Gems o' average brightness are\nworth 100\t, particularly bright gems are worth 250\t, & particu-\nlarly dark gems are worth 500\t."
                };
        }
    };

    Level::Level ( Level&& m )
    :
        id_ ( m.id_ ),
        maps_ ( std::move( m.maps_ ) ),
        goal_ ( std::move( m.goal_ ) ),
        entrance_x_ ( m.entrance_x_ ),
        entrance_y_ ( m.entrance_y_ ),
        camera_x_ ( m.camera_x_ ),
        camera_y_ ( m.camera_y_ ),
        message_ ( m.message_ )
    {};

    Level::Level
    (
        LevelName id,
        const std::vector<Map>& maps,
        std::unique_ptr<Goal> goal,
        int entrance_x,
        int entrance_y,
        int camera_x,
        int camera_y,
        std::string message
    )
    :
        id_ ( id ),
        maps_ ( std::move( maps ) ),
        goal_ ( std::move( goal ) ),
        entrance_x_ ( entrance_x ),
        entrance_y_ ( entrance_y ),
        camera_x_ ( camera_x ),
        camera_y_ ( camera_y ),
        message_ ( message ),
        current_map_ ( 0 )
    {};

    Level::~Level()
    {};

    Map& Level::currentMap()
    {
        return maps_.at( testCurrentMap() );
    };

    const Map& Level::currentMap() const
    {
        return maps_.at( testCurrentMap() );
    };

    int Level::testCurrentMap() const
    {
        assert ( current_map_ >= 0 && current_map_ < maps_.size() );

        return current_map_;
    };

    int Level::entranceX() const
    {
        return entrance_x_;
    };

    int Level::entranceY() const
    {
        return entrance_y_;
    };

    int Level::cameraX() const
    {
        return camera_x_;
    };

    int Level::cameraY() const
    {
        return camera_y_;
    };

    void Level::warp( SpriteSystem& sprites, Camera& camera, GameState* state, Graphics& graphics, InventoryLevel& inventory )
    {
        const Warp* warp = currentMap().getWarp( sprites.hero().xSubPixels(), sprites.hero().ySubPixels() );

        if ( warp != nullptr )
        {
            current_map_ = warp->mapNum();
			
			entrance_x_ = warp->entranceX();
			entrance_y_ = warp->entranceY();
			
            sprites.reset( *this, inventory );
            //sprites.hero().setPosition( warp->entranceX(), warp->entranceY() );

            int camera_x = camera.x();
            int camera_y = camera.y();

            if ( warp->cameraX() > -1 )
            {
                camera_x = warp->cameraX();
            }

            if ( warp->cameraY() > -1 )
            {
                camera_y = warp->cameraY();
            }

            camera.setPosition( camera_x, camera_y );
            camera.adjust( sprites.hero(), currentMap() );

            state->newPalette( graphics, currentMap().palette() );
			
			currentMap().setChanged();
        }
    };

    Goal* Level::goal() const
    {
        return goal_.get();
    }

    std::string Level::NameOLevel( int n )
    {
        switch ( n )
        {
            case ( LevelName::LV_CITY_1 ):
                return "Blueberry Burroughs";
                break;
            case ( LevelName::LV_CITY_2 ):
                return "Rooftop Rumble";
                break;
            case ( LevelName::LV_MINES_1 ):
                return "Minty Mines";
                break;
            case ( LevelName::LV_MINES_2 ):
                return "Curse o' the Ladder-Splayed Caves";
                break;
            case ( LevelName::LV_FACTORY_1 ):
                return "Steam Engenius";
                break;
            case ( LevelName::LV_FACTORY_2 ):
                return "Warm Up";
                break;
            case ( LevelName::LV_FACTORY_3 ):
                return "Level Industries";
                break;
            case ( LevelName::LV_MINES_4 ):
                return "The Minus Touch";
                break;
            case ( LevelName::LV_CITY_4 ):
                return "The Streets o' Storms";
                break;
            case ( LevelName::LV_SEWER_1 ):
                return "Soupy Sewers";
                break;
            case ( LevelName::LV_SNEAK ):
                return "Golden Gear Solid";
                break;
            case ( LevelName::LV_MOUNTAIN_1 ):
                return "Mustard Mountain";
                break;
            case ( LevelName::LV_DESERT_2 ):
                return "Dry Drought Desert";
                break;
            case ( LevelName::LV_SAW ):
                return "Sawdust Crush";
                break;
            case ( LevelName::LV_WOODS_1 ):
                return "Wasabi Woods";
                break;
            case ( LevelName::LV_PYRAMID_1 ):
                return "Pepperoncini Pyramid";
                break;
            case ( LevelName::LV_WOODS_2 ):
                return "Windy Woods";
                break;
            case ( LevelName::LV_WOODS_3 ):
                return "Bough Down";
                break;
            case ( LevelName::LV_DESERT_1 ):
                return "Desert Dare";
                break;
            case ( LevelName::LV_SNOW_1 ):
                return "Snowy Slopes";
                break;
            case ( LevelName::LV_SNOW_2 ):
                return "Frigid Frigates";
                break;
            case ( LevelName::LV_MAZE_1 ):
                return "Maybe I'm a Maze";
                break;
            case ( LevelName::LV_CART ):
                return "HOT SHOP";
                break;
            case ( LevelName::LV_SKY_1 ):
                return "OWL";
                break;
            case ( LevelName::LV_SKY_2 ):
                return "Value Valhalla";
                break;
            default:
                return "MISSINGNAME";
                break;
        }
    };

    std::string Level::message() const
    {
        return message_;
    };

    Level::LevelName Level::id() const
    {
        return id_;
    };

	void Level::interact( SpriteSystem& sprites )
	{
		currentMap().interact( sprites );
	};

	void Level::update( EventSystem& events )
	{
		currentMap().update( events );
	};