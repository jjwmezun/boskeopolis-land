#include "audio.hpp"
#include "block_system.hpp"
#include "level_message_state.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "map_layer_constellation_moving.hpp"
#include "map_layer_constellation_scrolling.hpp"
#include "map_layer_doom.hpp"
#include "map_layer_image.hpp"
#include "map_layer_image_switch.hpp"
#include "map_layer_lightning.hpp"
#include "map_layer_neon.hpp"
#include "map_layer_shade.hpp"
#include "map_layer_water.hpp"
#include "message_state.hpp"
#include "block_system.hpp"
#include "camera.hpp"
#include <cassert>
#include "clock.hpp"
#include <dirent.h>
#include "event_system.hpp"
#include <fstream>
#include "main.hpp"
#include "level.hpp"
#include "level_state.hpp"
#include "mezun_exceptions.hpp"
#include "mezun_helpers.hpp"
#include "rapidjson/istreamwrapper.h"
#include "sprite.hpp"
#include "sprite_system.hpp"
#include "wmessage_state.hpp"

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

static std::vector<std::string> level_list_;
static std::vector<int> gem_challenge_list_;
static std::vector<int> time_challenge_list_;
static std::vector<bool> has_secret_goals_;
static unsigned int real_level_num_ = 0;

static std::string code_names_[ Level::NUMBER_OF_LEVELS ] =
{
	""
};

static std::u32string level_names_[ Level::NUMBER_OF_LEVELS ] =
{
	U"MISSING LEVEL"
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
	message_ ( m.message_ ),
	current_map_ ( m.current_map_ ),
	start_on_ ( m.start_on_ )
{};

Level::Level
(
	int id,
	const std::vector<Map>& maps,
	std::unique_ptr<Goal> goal,
	int entrance_x,
	int entrance_y,
	int camera_x,
	int camera_y,
	std::u32string message,
	bool start_on
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
	current_map_ ( 0 ),
	start_on_ ( start_on )
{};

Level::~Level() {};

Map& Level::currentMap()
{
	return maps_.at( testCurrentMap() );
};

const Map& Level::currentMap() const
{
	return maps_.at( testCurrentMap() );
};

unsigned int Level::testCurrentMap() const
{
	assert ( current_map_ < maps_.size() );
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

void Level::warp( LevelState& level_state )
{
	SpriteSystem& sprites = level_state.sprites();
	const Warp* warp = currentMap().getWarp( sprites.hero().xSubPixels(), sprites.hero().ySubPixels() );
	if ( warp != nullptr )
	{
		Camera& camera = level_state.camera();
	
		currentMap().removeRenderableLayers( level_state );
		current_map_ = warp->mapNum();
		currentMap().initOtherLayers( level_state );
		entrance_x_ = warp->entranceX();
		entrance_y_ = warp->entranceY();

		sprites.reset( level_state );
		sprites.hero().hit_box_.x = Unit::PixelsToSubPixels( warp->entranceX() );
		sprites.hero().hit_box_.y = Unit::PixelsToSubPixels( warp->entranceY() );

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

		EventSystem& events = level_state.events();
		events.changePalette( currentMap().palette_ );
		if ( currentMap().auto_message_ )
		{
			events.showMessage();
		}
		level_state.blocks().reset( level_state );

		Audio::changeSong( currentMap().music_ );
	}
};

void Level::sewerWarp( SpriteSystem& sprites, EventSystem& events )
{
	current_map_ = ( current_map_ == 0 ) ? 1 : 0;
	events.changePalette( currentMap().palette_ );
	currentMap().changed_ = true;
};

const std::string& Level::NameOLevel( unsigned int n )
{
	assert( n < level_list_.size() );
	return level_list_.at( n );
};

int Level::gemChallenge( unsigned int n )
{
	assert( n < level_list_.size() );
	return gem_challenge_list_.at( n );
};

std::u32string Level::gemChallengeText( unsigned int n )
{
	return mezun::intToChar32StringWithPadding( gemChallenge( n ), 5 );
};

int Level::timeChallenge( unsigned int n )
{
	assert( n < level_list_.size() );
	return time_challenge_list_.at( n );
};

bool Level::hasSecretGoal( unsigned int n )
{
	if( n >= has_secret_goals_.size() )
	{
		return false;
	}
	return has_secret_goals_.at( n );
};

std::u32string Level::timeChallengeText( unsigned int n )
{
	return Clock::timeToString( timeChallenge( n ) );
};

const std::u32string& Level::message() const
{
	return message_;
};

int Level::id() const
{
	return id_;
};

std::string Level::getCodeName() const
{
	return Level::getCodeNameByID( id_ );
};

void Level::init( LevelState& level_state )
{
	goal_->init( level_state );
	initCurrentMap( level_state );
};

void Level::initCurrentMap( LevelState& level_state )
{
	currentMap().initOtherLayers( level_state );
};

void Level::updateGoal( LevelState& level_state )
{
	goal_->update( level_state );
};

unsigned int Level::realLevelNum()
{
	return real_level_num_;
};


Level Level::getLevel( int id )
{
	const std::string lvname = getCodeNameByID( id );
	const std::string file_path = Main::resourcePath() + "levels" + Main::pathDivider() + lvname + ".json";

	std::ifstream ifs( file_path );

	if( !ifs.is_open() )
	{
		throw mezun::MissingLevel( lvname );
	}

	rapidjson::IStreamWrapper ifs_wrap( ifs );
	rapidjson::Document lv;
	lv.ParseStream( ifs_wrap );

	if ( !lv.IsObject() )
	{
		throw mezun::BrokenLevelFile( lvname );
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

void Level::buildLevelList()
{
	const std::string path = Main::resourcePath() + "levels" + Main::pathDivider();

	if ( !mezun::checkDirectory( path ) )
	{
		throw mezun::CantLoadLevelNames();
	}

	int i = 0;
	for ( int cycle = 1; cycle <= NUMBER_OF_CYCLES; ++cycle )
	{
		for ( int theme_id = 0; theme_id < NUMBER_OF_THEMES; ++theme_id )
		{
			const std::string theme = Level::THEMES[ theme_id ];
			code_names_[ i ] = theme + "-" + std::to_string( cycle );
			level_names_[ i ] = Localization::getCurrentLanguage().getLevelName( code_names_[ i ] );

			const std::string file_path = path + code_names_[ i ]  + ".json";

			std::ifstream ifs( file_path );

			if ( ifs.is_open() )
			{
				rapidjson::IStreamWrapper ifs_wrap( ifs );
				rapidjson::Document lv;
				lv.ParseStream( ifs_wrap );

				if ( lv.IsObject() )
				{
					if ( lv.HasMember( "name" ) && lv[ "name" ].IsString() )
					{
						level_list_.emplace_back( lv[ "name" ].GetString() );
						++real_level_num_;
					}
					else
					{
						level_list_.emplace_back( "" );
					}

					if ( lv.HasMember( "gem_challenge" ) && lv[ "gem_challenge" ].IsInt() )
					{
						gem_challenge_list_.emplace_back( lv[ "gem_challenge" ].GetInt() );
					}
					else
					{
						gem_challenge_list_.emplace_back( 0 );
					}

					if ( lv.HasMember( "time_challenge" ) && lv[ "time_challenge" ].IsInt() )
					{
						time_challenge_list_.emplace_back( lv[ "time_challenge" ].GetInt() );
					}
					else
					{
						time_challenge_list_.emplace_back( 0 );
					}

					if ( lv.HasMember( "secret_goal" ) && lv[ "secret_goal" ].IsBool() )
					{
						has_secret_goals_.emplace_back( lv[ "secret_goal" ].GetBool() );
					}
					else
					{
						has_secret_goals_.emplace_back( false );
					}
				}
				else
				{
					//throw mezun::CorruptedLevel( code_names_[ i ] );
					Main::changeState( std::unique_ptr<GameState> ( WMessageState::generateErrorMessage( mezun::charToChar32String( std::string( "Level “" + code_names_[ i ] + "”’s JSON file in assets/levels has been corrupted. Please redownload game." ).c_str() ), WMessageState::Type::POP, nullptr ) ) );
				}

				ifs.close();
			}
			else
			{
				level_list_.emplace_back( "=BROKEN_LEVEL=" );
				gem_challenge_list_.emplace_back( 0 );
				time_challenge_list_.emplace_back( 0 );
			}
			++i;
		}
	}
};

void Level::regenerateLevelNames()
{
	for ( int i = 0; i < NUMBER_OF_LEVELS; ++i )
	{
		level_names_[ i ] = Localization::getCurrentLanguage().getLevelName( code_names_[ i ] );
	}
};

// For "Kill All Enemies" goal.
int Level::allEnemiesToKill() const
{
	int n = 0;
	for ( auto& m : maps_ )
	{
		for ( int i = 0; i < m.spritesSize(); ++i )
		{
			if
			(
				( m.sprite( i ) == 450 || m.sprite( i ) == 451 ) ||
				( m.sprite( i ) > 452 && m.sprite( i ) < 457 ) ||
				( m.sprite( i ) > 519 && m.sprite( i ) < 533 ) ||
				m.sprite( i ) == 577 || m.sprite( i ) == 578
			)
			{
				++n;
			}
		}
	}
	return n;
};

bool Level::startOn() const
{
	return start_on_;
};

std::string Level::getCodeNameByID( int id )
{
	return code_names_[ id ];
};

int Level::getIDFromCodeName( std::string code_name )
{
	for ( int i = 0; i < NUMBER_OF_LEVELS; ++i )
	{
		if ( code_name == code_names_[ i ] )
		{
			return i;
		}
	}
	assert( false );
};

const std::u32string* Level::getLevelNames()
{
	return level_names_;
};

const std::string& Level::getCodeName( int level_id )
{
	assert( level_id >= 0 && level_id < Level::NUMBER_OF_LEVELS );
	return code_names_[ level_id ];
};

const std::u32string& Level::getSpecialLevelName( int number )
{
	return getLevelNames()[ NUMBER_OF_THEMES * number - 1 ];
};

const int Level::getIDbyCycleAndTheme( int cycle, int theme )
{
	return cycle * NUMBER_OF_THEMES + theme;
};

const std::string& Level::getCodeNameByCycleAndTheme( int cycle, int theme )
{
	return code_names_[ getIDbyCycleAndTheme( cycle, theme ) ];
};