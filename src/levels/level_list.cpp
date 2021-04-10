#include "clock.hpp"
#include <fstream>
#include "level.hpp"
#include "level_data.hpp"
#include "level_list.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "mezun_exceptions.hpp"
#include "mezun_helpers.hpp"
#include "mezun_json.hpp"
#include "wmessage_state.hpp"

#include "map_layer_constellation_moving.hpp"
#include "map_layer_constellation_scrolling.hpp"
#include "map_layer_doom.hpp"
#include "map_layer_image.hpp"
#include "map_layer_image_switch.hpp"
#include "map_layer_lightning.hpp"
#include "map_layer_neon.hpp"
#include "map_layer_overlay.hpp"
#include "map_layer_rain.hpp"
#include "map_layer_rain_gradual.hpp"
#include "map_layer_shade.hpp"
#include "map_layer_water.hpp"

//GOALS
#include "avoid_money_goal.hpp"
#include "collect_goal.hpp"
#include "do_nothing_goal.hpp"
#include "heat_goal.hpp"
#include "kill_all_goal.hpp"
#include "mcguffin_goal.hpp"
#include "past_right_edge_goal.hpp"
#include "starving_goal.hpp"
#include "stop_on_off_goal.hpp"
#include "survive_time_goal.hpp"
#include "timed_goal.hpp"
#include "timed_on_goal.hpp"
#include "warp_goal.hpp"

static std::string directory_;
static std::vector<LevelData> levels_;
static std::string current_code_name_;

static void loadListFunction( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& root );
static void loadLevelDataFunction( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& root );

void LevelList::init()
{
    directory_ = Main::resourcePath() + "levels" + Main::pathDivider();

	if ( !mezun::checkDirectory( directory_ ) )
	{
        throw mezun::Exception( U"Can’t load assets/levels directory. Please redownload game." );
	}

    mezun::loadJSON
    (
        directory_ + "list.json",
        loadListFunction,
        mezun::charToChar32String( "Failed to load level list. Please redownload game & try ’gain" ),
        mezun::charToChar32String( "Level list has become corrupted. Please redownload game & try ’gain." )
    );
};

Level LevelList::getLevel( int id )
{
    if ( id < 0 || id >= levels_.size() )
    {
        throw mezun::Exception( mezun::stringReplace( U"Missing Level ID #%d.", U"%d", mezun::charToChar32String( std::to_string( id ).c_str() ) ) );
    };
    const std::string lvname = levels_[ id ].code_name_;
    const std::string file_path = directory_ + lvname + ".json";

    std::ifstream ifs( file_path );

    if( !ifs.is_open() )
    {
        throw mezun::Exception( mezun::string8ToString32( "Level “" + lvname + "” is missing its JSON file in the assets/levels directory.\nPlease redownload game." ) );
    }

    rapidjson::IStreamWrapper ifs_wrap( ifs );
    rapidjson::Document lv;
    lv.ParseStream( ifs_wrap );

    if ( !lv.IsObject() )
    {
        throw mezun::Exception( mezun::string8ToString32( "The JSON file for level “" + lvname + "” in the assets/levels directory has become corrupted & isn't valid JSON & can't be loaded.\nPlease redownload game." ) );
    }

    auto lvobj = lv.GetObject();

    /* MAP
    ==============================================================*/

    if ( !lvobj.HasMember( "maps" ) || !lvobj[ "maps" ].IsArray() )
    {
        throw mezun::BrokenLevelFile( lvname );
    }

    std::vector<Map> maps;
    std::string slug = "";

    for ( auto& m : lvobj[ "maps" ].GetArray() )
    {
        if ( m.IsObject() )
        {
            auto mobj = m.GetObject();

            if ( mobj.HasMember( "slug" ) && mobj[ "slug" ].IsString() )
            {
                slug = mobj[ "slug" ].GetString();
            }


        /* WARPS
        ==============================================================*/

                    std::vector<Warp> warps;

                    if ( mobj.HasMember( "warps" ) && mobj[ "warps" ].IsArray() )
                    {
                        for ( auto& w : mobj[ "warps" ].GetArray() )
                        {
                            if ( w.IsObject() )
                            {
                                auto wo = w.GetObject();

                                int map = 0;
                                int entrance_x = 0;
                                int entrance_y = 0;
                                int location[ 4 ] = { 0 };
                                int camera_x = -1;
                                int camera_y = -1;

                                if ( wo.HasMember( "map" ) && wo[ "map" ].IsInt() )
                                {
                                    map = wo[ "map" ].GetInt();
                                }

                                if ( wo.HasMember( "entrance_x" ) && wo[ "entrance_x" ].IsInt() )
                                {
                                    entrance_x = Unit::BlocksToPixels( wo[ "entrance_x" ].GetInt() );
                                }

                                if ( wo.HasMember( "entrance_y" ) && wo[ "entrance_y" ].IsInt() )
                                {
                                    entrance_y = Unit::BlocksToPixels( wo[ "entrance_y" ].GetInt() );
                                }

                                if ( wo.HasMember( "location" ) && wo[ "location" ].IsArray() )
                                {
                                    int li = 0;
                                    for ( auto& l : wo[ "location" ].GetArray() )
                                    {
                                        if ( l.IsInt() )
                                        {
                                            if ( li > 4 ) break;

                                            location[ li ] = l.GetInt();

                                            ++li;
                                        }
                                    }
                                }

                                if ( wo.HasMember( "camera_x" ) && wo[ "camera_x" ].IsInt() )
                                {
                                    camera_x = wo[ "camera_x" ].GetInt();
                                }

                                if ( wo.HasMember( "camera_y" ) && wo[ "camera_y" ].IsInt() )
                                {
                                    camera_y = wo[ "camera_y" ].GetInt();
                                }

                                warps.emplace_back( map, entrance_x, entrance_y, location[ 0 ], location[ 1 ], location[ 2 ], location[ 3 ], camera_x, camera_y );
                            }
                        }

                    }



        /* LAYERS
        ==============================================================*/

            std::vector<std::shared_ptr<MapLayer>> layers;
            if ( mobj.HasMember( "layers" ) && mobj[ "layers" ].IsArray() )
            {
                for ( auto& layer_item : mobj[ "layers" ].GetArray() )
                {
                    if ( layer_item.IsObject() )
                    {
                        auto layer = layer_item.GetObject();
                        if ( layer.HasMember( "type" ) && layer[ "type" ].IsString() )
                        {
                            auto layer_type = layer[ "type" ].GetString();
                            int layer_width = 128;
                            int layer_height = 128;
                            double layer_x_scroll = 1;
                            double layer_y_scroll = 1;
                            int layer_x_speed  = 0;
                            Unit::Layer layer_position = MapLayer::DEFAULT_POSITION;

                            if ( layer.HasMember( "layer" ) )
                            {
                                if ( layer[ "layer" ].IsInt() )
                                {
                                    layer_position = Unit::IntToLayer( layer[ "layer" ].GetInt() );
                                }
                                else if ( layer[ "layer" ].IsString() && mezun::areStringsEqual( layer[ "layer" ].GetString(), "Foreground" ) )
                                {
                                    layer_position = Unit::Layer::FG_1;
                                }
                            }

                            if ( layer.HasMember( "width" ) && layer[ "width" ].IsInt() )
                            {
                                layer_width = layer[ "width" ].GetInt();
                            }
                            if ( layer.HasMember( "height" ) && layer[ "height" ].IsInt() )
                            {
                                layer_height = layer[ "height" ].GetInt();
                            }

                            if ( layer.HasMember( "x_scroll" ) && layer[ "x_scroll" ].IsDouble() )
                            {
                                layer_x_scroll = layer[ "x_scroll" ].GetDouble();
                            }
                            else if ( layer.HasMember( "x_scroll" ) && layer[ "x_scroll" ].IsInt() )
                            {
                                layer_x_scroll = ( double )layer[ "x_scroll" ].GetInt();
                            }

                            if ( layer.HasMember( "y_scroll" ) && layer[ "y_scroll" ].IsDouble() )
                            {
                                layer_y_scroll = layer[ "y_scroll" ].GetDouble();
                            }
                            else if ( layer.HasMember( "y_scroll" ) && layer[ "y_scroll" ].IsInt() )
                            {
                                layer_x_scroll = ( double )layer[ "y_scroll" ].GetInt();
                            }

                            if ( mezun::areStringsEqual( layer_type, "image" ) )
                            {
                                std::string img = "clouds.png";
                                int bgxoffset = 0;
                                int bgyoffset = 0;
                                int bgframes  = 1;
                                int bgxrepeat = 255;
                                int bgyrepeat = 255;
                                int bgyspeed  = 0;
                                int bganimspeed = 1;
                                bool bganimflip = false;
                                Uint8 alpha = 255;
                                SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE;

                                if ( layer.HasMember( "img" ) && layer[ "img" ].IsString() )
                                {
                                    img = "bg" + Main::pathDivider() + layer[ "img" ].GetString();
                                }
                                if ( layer.HasMember( "alpha" ) && layer[ "alpha" ].IsInt() )
                                {
                                    alpha = ( Uint8 )( layer[ "alpha" ].GetInt() );
                                }
                                if ( layer.HasMember( "frames" ) && layer[ "frames" ].IsInt() )
                                {
                                    bgframes = layer[ "frames" ].GetInt();
                                }
                                if ( layer.HasMember( "x_offset" ) && layer[ "x_offset" ].IsInt() )
                                {
                                    bgxoffset = layer[ "x_offset" ].GetInt();
                                }
                                if ( layer.HasMember( "y_offset" ) && layer[ "y_offset" ].IsInt() )
                                {
                                    bgyoffset = layer[ "y_offset" ].GetInt();
                                }
                                if ( layer.HasMember( "x_repeat" ) )
                                {
                                    if ( layer[ "x_repeat" ].IsBool() && !layer[ "x_repeat" ].GetBool() )
                                    {
                                        bgxrepeat = 0;
                                    }
                                    else if ( layer[ "x_repeat" ].IsInt() )
                                    {
                                        bgxrepeat = layer[ "x_repeat" ].GetInt();
                                    }
                                }
                                if ( layer.HasMember( "y_repeat" ) )
                                {
                                    if ( layer[ "y_repeat" ].IsBool() && !layer[ "y_repeat" ].GetBool() )
                                    {
                                        bgyrepeat = 0;
                                    }
                                    else if ( layer[ "y_repeat" ].IsInt() )
                                    {
                                        bgyrepeat = layer[ "y_repeat" ].GetInt();
                                    }
                                }
                                if ( layer.HasMember( "frames" ) && layer[ "frames" ].IsInt() )
                                {
                                    bgframes = layer[ "frames" ].GetInt();
                                }
                                if ( layer.HasMember( "x_speed" ) && layer[ "x_speed" ].IsInt() )
                                {
                                    layer_x_speed = layer[ "x_speed" ].GetInt();
                                }
                                if ( layer.HasMember( "y_speed" ) && layer[ "y_speed" ].IsInt() )
                                {
                                    bgyspeed = layer[ "y_speed" ].GetInt();
                                }
                                if ( layer.HasMember( "animation_speed" ) && layer[ "animation_speed" ].IsInt() )
                                {
                                    bganimspeed = layer[ "animation_speed" ].GetInt();
                                }
                                if ( layer.HasMember( "animation_flip" ) && layer[ "animation_flip" ].IsBool() )
                                {
                                    bganimflip = layer[ "animation_flip" ].GetBool();
                                }
                                if ( layer.HasMember( "lighten" ) && layer[ "lighten" ].IsBool() && layer[ "lighten" ].GetBool() == true )
                                {
                                    blend_mode = SDL_BLENDMODE_ADD;
                                }
                                else if ( layer.HasMember( "darken" ) && layer[ "darken" ].IsBool() && layer[ "darken" ].GetBool() == true )
                                {
                                    blend_mode = SDL_BLENDMODE_MOD;
                                }

                                if ( layer.HasMember( "switch" ) && layer[ "switch" ].IsBool() && layer[ "switch" ].GetBool() == true )
                                {
                                    layers.emplace_back
                                    (
                                        std::make_shared<MapLayerImageSwitch>
                                        (
                                            std::forward<std::string> ( img ),
                                            layer_width,
                                            layer_height,
                                            bgxoffset,
                                            bgyoffset,
                                            layer_x_scroll,
                                            layer_y_scroll,
                                            bgframes,
                                            bgxrepeat,
                                            bgyrepeat,
                                            layer_x_speed,
                                            bgyspeed,
                                            bganimspeed,
                                            bganimflip,
                                            alpha,
                                            blend_mode,
                                            layer_position
                                        )
                                    );
                                }
                                else
                                {
                                    layers.emplace_back
                                    (
                                        std::make_shared<MapLayerImage>
                                        (
                                            std::forward<std::string> ( img ),
                                            layer_width,
                                            layer_height,
                                            bgxoffset,
                                            bgyoffset,
                                            layer_x_scroll,
                                            layer_y_scroll,
                                            bgframes,
                                            bgxrepeat,
                                            bgyrepeat,
                                            layer_x_speed,
                                            bgyspeed,
                                            bganimspeed,
                                            bganimflip,
                                            alpha,
                                            blend_mode,
                                            layer_position
                                        )
                                    );
                                }

                            }
                            else if ( mezun::areStringsEqual( layer_type, "constellation" ) )
                            {
                                double move_speed = 0.0;
                                std::string image = "constellation.png";

                                if ( layer.HasMember( "image" ) && layer[ "image" ].IsString() )
                                {
                                    image = layer[ "image" ].GetString();
                                }

                                if ( layer.HasMember( "version" ) && layer[ "version" ].IsString() && strcmp( layer[ "version" ].GetString(), "moving" ) == 0 )
                                {
                                    if ( layer.HasMember( "move_speed" ) && layer[ "move_speed" ].IsFloat() )
                                    {
                                        move_speed = layer[ "move_speed" ].GetFloat();
                                    }
                                    layers.emplace_back
                                    (
                                        std::make_shared<MapLayerConstellationMoving>
                                        (
                                            layer_width,
                                            layer_height,
                                            move_speed,
                                            image,
                                            layer_position
                                        )
                                    );
                                }
                                else
                                {
                                    layers.emplace_back
                                    (
                                        std::make_shared<MapLayerConstellationScrolling>
                                        (
                                            layer_width,
                                            layer_height,
                                            image,
                                            layer_position
                                        )
                                    );
                                }
                            }
                            else if ( mezun::areStringsEqual( layer_type, "shade" ) )
                            {
                                Uint8 alpha = 255;
                                if ( layer.HasMember( "color" ) && layer[ "color" ].IsInt() )
                                {
                                    if ( layer.HasMember( "alpha" ) && layer[ "alpha" ].IsInt() )
                                    {
                                        alpha = ( Uint8 )( layer[ "alpha" ].GetInt() );
                                    }

                                    layers.emplace_back
                                    (
                                        std::make_shared<MapLayerShade> ( layer[ "color" ].GetInt(), alpha, layer_position )
                                    );
                                }
                            }
                            else if ( mezun::areStringsEqual( layer_type, "neon" ) )
                            {
                                layers.emplace_back
                                (
                                    std::make_shared<MapLayerNeon> ( layer_position )
                                );
                            }
                            else if ( mezun::areStringsEqual( layer_type, "lightning" ) )
                            {
                                layers.emplace_back
                                (
                                    std::make_shared<MapLayerLightning> ( layer_position )
                                );
                            }
                            else if ( mezun::areStringsEqual( layer_type, "doom" ) )
                            {
                                layers.emplace_back
                                (
                                    std::make_shared<MapLayerDoom> ( layer_position )
                                );
                            }
                            else if ( mezun::areStringsEqual( layer_type, "overlay" ) )
                            {
                                const Uint8 red = ( Uint8 )( ( layer.HasMember( "red" ) && layer[ "red" ].IsInt() ) ? layer[ "red" ].GetInt() : 255 );
                                const Uint8 green = ( Uint8 )( ( layer.HasMember( "green" ) && layer[ "green" ].IsInt() ) ? layer[ "green" ].GetInt() : 255 );
                                const Uint8 blue = ( Uint8 )( ( layer.HasMember( "blue" ) && layer[ "blue" ].IsInt() ) ? layer[ "blue" ].GetInt() : 255 );
                                const Uint8 alpha = ( Uint8 )( ( layer.HasMember( "alpha" ) && layer[ "alpha" ].IsInt() ) ? layer[ "alpha" ].GetInt() : 255 );
                                layers.emplace_back
                                (
                                    std::make_shared<MapLayerOverlay> ( red, green, blue, alpha, layer_position )
                                );
                            }
                            else if ( mezun::areStringsEqual( layer_type, "rain" ) )
                            {
                                layers.emplace_back
                                (
                                    std::make_shared<MapLayerRain> ( layer_position )
                                );
                            }
                            else if ( mezun::areStringsEqual( layer_type, "rain_gradual" ) )
                            {
                                const int start = ( layer.HasMember( "start" ) && layer[ "start" ].IsInt() ) ? layer[ "start" ].GetInt() : -1;
                                const int end = ( layer.HasMember( "end" ) && layer[ "end" ].IsInt() ) ? layer[ "end" ].GetInt() : -1;
                                layers.emplace_back
                                (
                                    std::make_shared<MapLayerRainGradual> ( start, end, layer_position )
                                );
                            }
                        }
                    }
                }
            }

            maps.push_back
            (
                Map::mapFromPath
                (
                    slug,
                    layers,
                    warps
                )
            );

        }
    }



    /* GOAL
    ==============================================================*/

    std::unique_ptr<Goal> goal;
    std::u32string goal_message = Localization::getCurrentLanguage().getLevelGoalMessage( lvname );
    if ( lvobj.HasMember( "goal" ) && lvobj[ "goal" ].IsObject() )
    {
        auto lvg = lvobj[ "goal" ].GetObject();
        if ( lvg.HasMember( "type" ) && lvg[ "type" ].IsString() )
        {
            auto goaltype = lvg[ "type" ].GetString();
            if ( mezun::areStringsEqual( goaltype, "collect" ) )
            {
                if ( goal_message == U"" )
                {
                    goal_message = Localization::getCurrentLanguage().getCollectGoalMessage();
                }
                goal = ( lvg.HasMember( "amount" ) && lvg[ "amount" ].IsInt() )
                    ? std::make_unique<CollectGoal> ( goal_message, lvg[ "amount" ].GetInt() )
                    : std::make_unique<CollectGoal> ( goal_message );
            }
            else if ( mezun::areStringsEqual( goaltype, "mcguffin" ) )
            {
                if ( goal_message == U"" )
                {
                    goal_message = Localization::getCurrentLanguage().getMcGuffinGoalMessage();
                }
                goal = ( lvg.HasMember( "amount" ) && lvg[ "amount" ].IsInt() )
                    ? std::make_unique<McGuffinGoal> ( goal_message, lvg[ "amount" ].GetInt() )
                    : std::make_unique<McGuffinGoal> ( goal_message );
            }
            else if ( mezun::areStringsEqual( goaltype, "survive_time" ) )
            {
                if ( goal_message == U"" )
                {
                    goal_message = Localization::getCurrentLanguage().getSurviveTimeGoalMessage();
                }
                goal = ( lvg.HasMember( "amount" ) && lvg[ "amount" ].IsInt() )
                    ? std::make_unique<SurviveTimeGoal> ( goal_message, lvg[ "amount" ].GetInt() )
                    : std::make_unique<SurviveTimeGoal> ( goal_message );
            }
            else if ( mezun::areStringsEqual( goaltype, "past_right_edge" ) )
            {
                if ( goal_message == U"" )
                {
                    goal_message = Localization::getCurrentLanguage().getPastRightEdgeGoalMessage();
                }
                goal = std::make_unique<PastRightEdgeGoal> ( goal_message );
            }
            else if ( mezun::areStringsEqual( goaltype, "warp" ) )
            {
                if ( goal_message == U"" )
                {
                    goal_message = Localization::getCurrentLanguage().getWarpGoalMessage();
                }
                goal = std::make_unique<WarpGoal> ( goal_message );
            }
            else if ( mezun::areStringsEqual( goaltype, "avoid_money" ) )
            {
                if ( goal_message == U"" )
                {
                    goal_message = Localization::getCurrentLanguage().getAvoidMoneyGoalMessage();
                }
                goal = std::make_unique<AvoidMoneyGoal> ( goal_message );
            }
            else if ( mezun::areStringsEqual( goaltype, "starving" ) )
            {
                if ( goal_message == U"" )
                {
                    goal_message = Localization::getCurrentLanguage().getStarvingGoalMessage();
                }
                const int starting_amount = ( lvg.HasMember( "starting_amount" ) && lvg[ "starting_amount" ].IsInt() )
                    ? lvg[ "starting_amount" ].GetInt()
                    : StarvingGoal::DEFAULT_STARTING_AMOUNT;
                const int amount_lost_per_frame = ( lvg.HasMember( "amount_lost_per_frame" ) && lvg[ "amount_lost_per_frame" ].IsInt() )
                    ? lvg[ "amount_lost_per_frame" ].GetInt()
                    : StarvingGoal::DEFAULT_AMOUNT_LOST_PER_FRAME;
                goal = std::make_unique<StarvingGoal> ( goal_message, starting_amount, amount_lost_per_frame );
            }
            else if ( mezun::areStringsEqual( goaltype, "heat" ) )
            {
                if ( goal_message == U"" )
                {
                    goal_message = Localization::getCurrentLanguage().getHeatGoalMessage();
                }
                goal = std::make_unique<HeatGoal> ( goal_message );
            }
            else if ( mezun::areStringsEqual( goaltype, "do_nothing" ) )
            {
                if ( goal_message == U"" )
                {
                    goal_message = Localization::getCurrentLanguage().getDoNothingGoalMessage();
                }
                goal = ( lvg.HasMember( "amount" ) && lvg[ "amount" ].IsInt() )
                    ? std::make_unique<DoNothingGoal> ( goal_message, lvg[ "amount" ].GetInt() )
                    : std::make_unique<DoNothingGoal> ( goal_message );
            }
            else if ( mezun::areStringsEqual( goaltype, "kill_all" ) )
            {
                if ( goal_message == U"" )
                {
                    goal_message = Localization::getCurrentLanguage().getKillAllGoalMessage();
                }
                goal = std::make_unique<KillAllGoal> ( goal_message );
            }
            else if ( mezun::areStringsEqual( goaltype, "stop_on_off" ) )
            {
                if ( goal_message == U"" )
                {
                    goal_message = Localization::getCurrentLanguage().getStopOnOffGoalMessage();
                }
                goal = std::make_unique<StopOnOffGoal> ( goal_message );
            }
            else if ( mezun::areStringsEqual( goaltype, "timed_on" ) )
            {
                if ( goal_message == U"" )
                {
                    goal_message = Localization::getCurrentLanguage().getGenericGoalMessage();
                }
                goal = ( lvg.HasMember( "time" ) && lvg[ "time" ].IsInt() )
                    ? std::make_unique<TimedOnGoal> ( goal_message, lvg[ "time" ].GetInt() )
                    : std::make_unique<TimedOnGoal> ( goal_message );
            }
            else if ( mezun::areStringsEqual( goaltype, "timed" ) )
            {
                if ( goal_message == U"" )
                {
                    goal_message = Localization::getCurrentLanguage().getStopOnOffGoalMessage();
                }
                goal = ( lvg.HasMember( "time_limit" ) && lvg[ "time_limit" ].IsInt() )
                    ? std::make_unique<TimedGoal> ( goal_message, lvg[ "time_limit" ].GetInt() )
                    : std::make_unique<TimedGoal> ( goal_message );
            }
        }
    }
    else
    {
        if ( goal_message == U"" )
        {
            goal_message = Localization::getCurrentLanguage().getGenericGoalMessage();
        }
        goal = std::make_unique<Goal> ( goal_message );
    }




    /* ENTRANCES
    ==============================================================*/
    int entrance_x = 0;
    int entrance_y = 0;

    if ( lvobj.HasMember( "entrance_x" ) && lvobj[ "entrance_x" ].IsInt() )
    {
        entrance_x = Unit::BlocksToPixels( lvobj[ "entrance_x" ].GetInt() );
    }
    if ( lvobj.HasMember( "entrance_y" ) && lvobj[ "entrance_y" ].IsInt() )
    {
        entrance_y = Unit::BlocksToPixels( lvobj[ "entrance_y" ].GetInt() );
    }



    /* CAMERA
    ==============================================================*/
    int camera_x = 0;
    int camera_y = 0;

    if ( lvobj.HasMember( "camera_x" ) && lvobj[ "camera_x" ].IsInt() )
    {
        camera_x = Unit::BlocksToPixels( lvobj[ "camera_x" ].GetInt() );
    }
    if ( lvobj.HasMember( "camera_y" ) && lvobj[ "camera_y" ].IsInt() )
    {
        camera_y = Unit::BlocksToPixels( lvobj[ "camera_y" ].GetInt() );
    }



    /* CAMERA
    ==============================================================*/
    const std::u32string message = Localization::getCurrentLanguage().getLevelMessage( lvname );



    /* OTHER
    ==============================================================*/
    const bool start_on = ( lvobj.HasMember( "start_on" ) && lvobj[ "start_on" ].IsBool() && lvobj[ "start_on" ].GetBool() );



    /* CONCLUDE
    ==============================================================*/
    ifs.close();
    return Level( id, maps, std::move( goal ), entrance_x, entrance_y, camera_x, camera_y, message, start_on );
};

const std::vector<LevelData>& LevelList::getLevelDataList()
{
    return levels_;
};

int LevelList::getNumberOfLevels()
{
    return levels_.size();
};

int LevelList::gemChallenge( unsigned int level )
{
    return levels_[ level ].gem_challenge_;
};

std::u32string LevelList::gemChallengeText( unsigned int level )
{
    return mezun::intToChar32StringWithPadding( gemChallenge( level ), 5 );
};

int LevelList::timeChallenge( unsigned int level )
{
    return levels_[ level ].time_challenge_;
};

std::u32string LevelList::timeChallengeText( unsigned int level )
{
    return Clock::timeToString( timeChallenge( level ) );
};

bool LevelList::hasSecretGoal( unsigned int level )
{
    return levels_[ level ].has_secret_goal_;
};

int LevelList::getIDFromCodeName( std::string code_name )
{
	for ( int i = 0; i < levels_.size(); ++i )
	{
		if ( code_name == levels_[ i ].code_name_ )
		{
			return i;
		}
	}
    Main::pushState( WMessageState::generateErrorMessage( mezun::stringReplace( mezun::charToChar32String( "Missing level data for code name “%c”. Please redownload game." ), U"%c", mezun::charToChar32String( code_name.c_str() ) ), WMessageState::Type::POP, nullptr ) );
    return -1;
};

const std::string& LevelList::getCodeNameFromID( unsigned int level )
{
	return levels_[ level ].code_name_;
};

int LevelList::getSpecialLevelID( unsigned int number )
{
    return getIDbyCycleAndTheme( number, 16 );
};

int LevelList::getIDbyCycleAndTheme( unsigned int cycle, unsigned int theme )
{
	for ( int i = 0; i < levels_.size(); ++i )
	{
		if ( levels_[ i ].theme_ == theme && levels_[ i ].cycle_ == cycle )
		{
			return i;
		}
	}
    Main::pushState( WMessageState::generateErrorMessage( mezun::string8ToString32( "Missing level data for level with cycle #" + std::to_string( cycle ) + " & theme #" + std::to_string( theme ) + ". Please redownload game." ), WMessageState::Type::POP, nullptr ) );
    return -1;
};

const std::string& LevelList::getCodeNameByCycleAndTheme( unsigned int cycle, unsigned int theme )
{
	return levels_[ getIDbyCycleAndTheme( cycle, theme ) ].code_name_;
};

int LevelList::getCycleFromLevelID( unsigned int level )
{
	return levels_[ level ].cycle_;
};

int LevelList::getThemeFromLevelID( unsigned int level )
{
	return levels_[ level ].theme_;
};

std::u32string LevelList::getThemeCodeFromLevelID( unsigned int level )
{
	return Localization::getCurrentLanguage().getThemeCode( getThemeFromLevelID( level ) - 1 );
};

std::u32string LevelList::getLevelHeader( unsigned int level )
{
    const unsigned int theme = getThemeFromLevelID( level );
    const unsigned int cycle = getCycleFromLevelID( level );
    if ( !levels_[ level ].show_cycle_ || !levels_[ level ].show_theme_ || theme == 0 || cycle == 0 )
    {
        return U"";
    }

	return mezun::charToChar32String
	(
		mezun::stringReplace
		(
			mezun::stringReplace( "%c-%t: ", "%c", std::to_string( getCycleFromLevelID( level ) ) ),
			"%t",
			mezun::string32ToString8( getThemeCodeFromLevelID( level ) )
		).c_str()
	);
};

int LevelList::getNextLevel( unsigned int level )
{
    return levels_[ level ].next_level_;
};

bool LevelList::hasGemScore( unsigned int level )
{
    return levels_[ level ].gem_challenge_ > 0;
};

bool LevelList::hasTimeScore( unsigned int level )
{
    return levels_[ level ].time_challenge_ > 0;
};

bool LevelList::hasCard( unsigned int level )
{
    return levels_[ level ].has_card_;
};

bool LevelList::hasHardMode( unsigned int level )
{
    return levels_[ level ].has_crown_;
};

bool LevelList::hasSuits( unsigned int level )
{
    return levels_[ level ].has_suits_;
};

void loadListFunction( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& root )
{
    if ( root.HasMember( "levels" ) & root[ "levels" ].IsArray() )
    {
        for ( const auto& item : root[ "levels" ].GetArray() )
        {
            if ( item.IsString() )
            {
                current_code_name_ = item.GetString();
                mezun::loadJSON
                (
                    directory_ + current_code_name_ + ".json",
                    loadLevelDataFunction,
                    mezun::charToChar32String( std::string( "Level “" + current_code_name_ + "”’s JSON file cannot be found. Please redownload game." ).c_str() ),
                    mezun::charToChar32String( std::string( "Level “" + current_code_name_ + "”’s JSON file in assets/levels has been corrupted. Please redownload game." ).c_str() )
                );
            }
        }
    }
};

void loadLevelDataFunction( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& root )
{
    LevelData level;
    level.code_name_ = current_code_name_;
    level.cycle_ = mezun::JSONTryInt( root, "cycle", 0 );
    level.theme_ = mezun::JSONTryInt( root, "theme", 0 );;
    level.id_ = levels_.size();
    level.time_challenge_ = mezun::JSONTryInt( root, "time_challenge", 0 );
    level.gem_challenge_ = mezun::JSONTryInt( root, "gem_challenge", 0 );
    level.has_card_ = mezun::JSONTryBool( root, "has_card", true );
    level.has_crown_ = mezun::JSONTryBool( root, "has_crown", false );
    level.show_cycle_ = mezun::JSONTryBool( root, "show_cycle", true );
    level.show_theme_ = mezun::JSONTryBool( root, "show_theme", true );
    level.has_suits_ = mezun::JSONTryBool( root, "has_suits", true );
    level.has_secret_goal_ = mezun::JSONTryBool( root, "secret_goal", false );
    levels_.emplace_back( level );
};