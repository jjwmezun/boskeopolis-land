#include "map_layer_constellation.hpp"
#include "map_layer_image.hpp"
#include "map_layer_water.hpp"
#include "block_system.hpp"
#include "camera.hpp"
#include <cassert>
#include "clock.hpp"
#include <dirent.h>
#include "event_system.hpp"
#include <fstream>
#include "main.hpp"
#include "level.hpp"
#include "mezun_exceptions.hpp"
#include "rapidjson/istreamwrapper.h"
#include "sprite.hpp"
#include "sprite_system.hpp"
#include "text.hpp"

//GOALS
#include "avoid_money_goal.hpp"
#include "collect_goal.hpp"
#include "do_nothing_goal.hpp"
#include "heat_goal.hpp"
#include "kill_all_goal.hpp"
#include "mcguffin_goal.hpp"
#include "past_right_edge_goal.hpp"
#include "starving_goal.hpp"
#include "survive_time_goal.hpp"
#include "timed_goal.hpp"
#include "windy_goal.hpp"

std::vector<std::string> Level::level_list_;
std::vector<int> Level::gem_challenge_list_;
std::vector<int> Level::time_challenge_list_;
unsigned int Level::real_level_num_ = 0;

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
	current_map_ ( m.current_map_ )
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

void Level::warp( SpriteSystem& sprites, Camera& camera, EventSystem& events, BlockSystem& blocks )
{
	const Warp* warp = currentMap().getWarp( sprites.hero().xSubPixels(), sprites.hero().ySubPixels() );

	if ( warp != nullptr )
	{
		current_map_ = warp->mapNum();

		entrance_x_ = warp->entranceX();
		entrance_y_ = warp->entranceY();

		sprites.reset( *this );

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

		events.changePalette( currentMap().palette() );
		blocks.changedMap( currentMap() );
		Sprite::resistance_x_ = currentMap().windStrength();
		currentMap().setChanged();
	}
};

const std::string& Level::NameOLevel( unsigned int n )
{
	if ( level_list_.empty() )
	{
		buildLevelList();
	}

	assert( n < level_list_.size() );

	return level_list_.at( n );
};

int Level::gemChallenge( unsigned int n )
{
	if ( level_list_.empty() )
	{
		buildLevelList();
	}

	assert( n < level_list_.size() );

	return gem_challenge_list_.at( n );
};

std::string Level::gemChallengeText( unsigned int n )
{
	return Text::formatNumDigitPadding( gemChallenge( n ), 5 );
};

int Level::timeChallenge( unsigned int n )
{
	if ( level_list_.empty() )
	{
		buildLevelList();
	}

	assert( n < level_list_.size() );

	return time_challenge_list_.at( n );
};

std::string Level::timeChallengeText( unsigned int n )
{
	return Clock::timeToString( timeChallenge( n ) );
};

std::string Level::message() const
{
	return message_;
};

int Level::id() const
{
	return id_;
};

void Level::init( Sprite& hero, InventoryLevel& inventory, EventSystem& events, Health& health )
{
	goal_->init( hero, *this, inventory, events, health );
};

void Level::update( InventoryLevel& inventory_screen, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, const Camera& camera, Health& health )
{
	currentMap().update( events, sprites, blocks, camera );
	goal_->update( sprites, currentMap(), inventory_screen, events, health );
};

unsigned int Level::realLevelNum()
{
	if ( real_level_num_ == 0 )
	{
		buildLevelList();
	}

	return real_level_num_;
};


Level Level::getLevel( int id )
{
	const std::string file_path = Main::resourcePath() + "levels" + Main::pathDivider() + Text::formatNumDigitPadding( id, 3 ) + ".json";

	std::ifstream ifs( file_path );

	assert( ifs.is_open() );

	rapidjson::IStreamWrapper ifs_wrap( ifs );
	rapidjson::Document lv;
	lv.ParseStream( ifs_wrap );

	assert( lv.IsObject() );

	auto lvobj = lv.GetObject();

	/* MAP
	==============================================================*/

	assert( lvobj.HasMember( "maps" ) && lvobj[ "maps" ].IsArray() );

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

			// Can't rapidjson object to function, so we have to use dumb loops.								
			std::vector<std::string> map_layer_group_types = { "backgrounds", "foregrounds" };
			std::map<std::string, std::vector<std::unique_ptr<MapLayer>>> map_layer_groups;

			for ( auto& mlg : map_layer_group_types )
			{
				std::vector<std::unique_ptr<MapLayer>> group;
				const char* mlgc = mlg.c_str();

				if ( mobj.HasMember( mlgc ) && mobj[ mlgc ].IsArray() )
				{

					for ( auto& bgitem : mobj[ mlgc ].GetArray() )
					{

						if ( bgitem.IsObject() )
						{
							auto bg = bgitem.GetObject();

							if ( bg.HasMember( "type" ) && bg[ "type" ].IsString() )
							{

								auto bgtype = bg[ "type" ].GetString();

								int bgw = 128;
								int bgh = 128;
								double bgxscroll = 1;
								double bgyscroll = 1;


								if ( bg.HasMember( "width" ) && bg[ "width" ].IsInt() )
								{
									bgw = bg[ "width" ].GetInt();
								}
								if ( bg.HasMember( "height" ) && bg[ "height" ].IsInt() )
								{
									bgh = bg[ "height" ].GetInt();
								}

								if ( bg.HasMember( "x_scroll" ) && bg[ "x_scroll" ].IsDouble() )
								{
									bgxscroll = bg[ "x_scroll" ].GetDouble();
								}
								else if ( bg.HasMember( "x_scroll" ) && bg[ "x_scroll" ].IsInt() )
								{
									bgxscroll = ( double )bg[ "x_scroll" ].GetInt();
								}

								if ( bg.HasMember( "y_scroll" ) && bg[ "y_scroll" ].IsDouble() )
								{
									bgyscroll = bg[ "y_scroll" ].GetDouble();
								}
								else if ( bg.HasMember( "y_scroll" ) && bg[ "y_scroll" ].IsInt() )
								{
									bgxscroll = ( double )bg[ "y_scroll" ].GetInt();
								}

								if ( mezun::areStringsEqual( bgtype, "image" ) )
								{
									std::string img = "clouds.png";
									int bgxoffset = 0;
									int bgyoffset = 0;
									int bgframes  = 1;
									int bgxrepeat = 255;
									int bgyrepeat = 255;
									int bgxspeed  = 0;
									int bgyspeed  = 0;
									int bganimspeed = 1;
									bool bganimflip = false;
									Uint8 alpha = 255;

									if ( bg.HasMember( "img" ) && bg[ "img" ].IsString() )
									{
										img = "bg" + Main::pathDivider() + bg[ "img" ].GetString();
									}
									if ( bg.HasMember( "alpha" ) && bg[ "alpha" ].IsInt() )
									{
										alpha = ( Uint8 )( bg[ "alpha" ].GetInt() );
									}
									if ( bg.HasMember( "frames" ) && bg[ "frames" ].IsInt() )
									{
										bgframes = bg[ "frames" ].GetInt();
									}
									if ( bg.HasMember( "x_offset" ) && bg[ "x_offset" ].IsInt() )
									{
										bgxoffset = bg[ "x_offset" ].GetInt();
									}
									if ( bg.HasMember( "y_offset" ) && bg[ "y_offset" ].IsInt() )
									{
										bgyoffset = bg[ "y_offset" ].GetInt();
									}
									if ( bg.HasMember( "x_repeat" ) )
									{
										if ( bg[ "x_repeat" ].IsBool() && !bg[ "x_repeat" ].GetBool() )
										{
											bgxrepeat = 0;
										}
										else if ( bg[ "x_repeat" ].IsInt() )
										{
											bgxrepeat = bg[ "x_repeat" ].GetInt();
										}
									}
									if ( bg.HasMember( "y_repeat" ) )
									{
										if ( bg[ "y_repeat" ].IsBool() && !bg[ "y_repeat" ].GetBool() )
										{
											bgyrepeat = 0;
										}
										else if ( bg[ "y_repeat" ].IsInt() )
										{
											bgyrepeat = bg[ "y_repeat" ].GetInt();
										}
									}
									if ( bg.HasMember( "frames" ) && bg[ "frames" ].IsInt() )
									{
										bgframes = bg[ "frames" ].GetInt();
									}
									if ( bg.HasMember( "x_speed" ) && bg[ "x_speed" ].IsInt() )
									{
										bgxspeed = bg[ "x_speed" ].GetInt();
									}
									if ( bg.HasMember( "y_speed" ) && bg[ "y_speed" ].IsInt() )
									{
										bgyspeed = bg[ "y_speed" ].GetInt();
									}
									if ( bg.HasMember( "animation_speed" ) && bg[ "animation_speed" ].IsInt() )
									{
										bganimspeed = bg[ "animation_speed" ].GetInt();
									}
									if ( bg.HasMember( "animation_flip" ) && bg[ "animation_flip" ].IsBool() )
									{
										bganimflip = bg[ "animation_flip" ].GetBool();
									}


									group.emplace_back
									(
										std::make_unique<MapLayerImage>
										(
											std::forward<std::string> ( img ),
											bgw,
											bgh,
											bgxoffset,
											bgyoffset,
											bgxscroll,
											bgyscroll,
											bgframes,
											bgxrepeat,
											bgyrepeat,
											bgxspeed,
											bgyspeed,
											bganimspeed,
											bganimflip,
											alpha
										)
									);

								}
								else if ( mezun::areStringsEqual( bgtype, "constellation" ) )
								{
									group.emplace_back
									(
										std::make_unique<MapLayerConstellation>
										(
											bgw,
											bgh,
											.1,
											.1
										)
									);
								}

							}

						}

					}

				}

				map_layer_groups.insert( std::make_pair ( mlg, std::move( group ) ) );
			}




			maps.push_back
			(
				Map::mapFromPath
				(
					slug,
					std::move( map_layer_groups.at( "backgrounds" ) ),
					warps,
					std::move( map_layer_groups.at( "foregrounds" ) )
				)
			);



				}
			}



	/* GOAL
	==============================================================*/
	std::unique_ptr<Goal> goal;

	if ( lvobj.HasMember( "goal" ) && lvobj[ "goal" ].IsObject() )
	{
		auto lvg = lvobj[ "goal" ].GetObject();

		if ( lvg.HasMember( "type" ) && lvg[ "type" ].IsString() )
		{
			auto goaltype = lvg[ "type" ].GetString();

			if ( mezun::areStringsEqual( goaltype, "collect" ) )
			{
				int amount = 10000;
				std::string message = mezun::emptyString();

				if ( lvg.HasMember( "amount" ) && lvg[ "amount" ].IsInt() )
				{
					amount = lvg[ "amount" ].GetInt();
				}

				if ( lvg.HasMember( "message" ) && lvg[ "message" ].IsString() )
				{
					message = lvg[ "message" ].GetString();
				}

				goal = std::make_unique<CollectGoal> ( amount, message );
			}
			else if ( mezun::areStringsEqual( goaltype, "mcguffin" ) )
			{
				int amount = 3;
				std::string message = mezun::emptyString();

				if ( lvg.HasMember( "amount" ) && lvg[ "amount" ].IsInt() )
				{
					amount = lvg[ "amount" ].GetInt();
				}

				if ( lvg.HasMember( "message" ) && lvg[ "message" ].IsString() )
				{
					message = lvg[ "message" ].GetString();
				}

				goal = std::make_unique<McGuffinGoal> ( amount, message );
			}
			else if ( mezun::areStringsEqual( goaltype, "survive_time" ) )
			{
				int amount = 60;
				std::string message = mezun::emptyString();

				if ( lvg.HasMember( "amount" ) && lvg[ "amount" ].IsInt() )
				{
					amount = lvg[ "amount" ].GetInt();
				}

				if ( lvg.HasMember( "message" ) && lvg[ "message" ].IsString() )
				{
					message = lvg[ "message" ].GetString();
				}

				goal = std::make_unique<SurviveTimeGoal> ( amount, message );
			}
			else if ( mezun::areStringsEqual( goaltype, "past_right_edge" ) )
			{
				std::string message = mezun::emptyString();

				if ( lvg.HasMember( "message" ) && lvg[ "message" ].IsString() )
				{
					message = lvg[ "message" ].GetString();
				}

				goal = std::make_unique<PastRightEdgeGoal> ( message );
			}
			else if ( mezun::areStringsEqual( goaltype, "avoid_money" ) )
			{
				goal = std::make_unique<AvoidMoneyGoal> ();
			}
			else if ( mezun::areStringsEqual( goaltype, "starving" ) )
			{
				goal = std::make_unique<StarvingGoal> ();
			}
			else if ( mezun::areStringsEqual( goaltype, "heat" ) )
			{
				goal = std::make_unique<HeatGoal> ();
			}
			else if ( mezun::areStringsEqual( goaltype, "kill_all" ) )
			{
				goal = std::make_unique<KillAllGoal> ();
			}
		}
		else if ( lvg.HasMember( "message" ) && lvg[ "message" ].IsString() )
		{
			const std::string message = lvg[ "message" ].GetString();
			goal = std::make_unique<Goal> ( message );
		}
	}
	else
	{
		goal = std::make_unique<Goal> ();
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
	std::string message = "";

	if ( lvobj.HasMember( "message" ) && lvobj[ "message" ].IsString() )
	{
		message = lvobj[ "message" ].GetString();
	}



	/* CONCLUDE
	==============================================================*/
	ifs.close();
	return Level( id, maps, std::move( goal ), entrance_x, entrance_y, camera_x, camera_y, message );
};

void Level::buildLevelList()
{

	const std::string path = Main::resourcePath() + "levels" + Main::pathDivider();

	if ( !mezun::checkDirectory( path ) )
	{
		throw mezun::CantLoadLevels();
	}

	for ( int i = 0; i < NUM_O_LEVELS; ++i )
	{

		const std::string file_path = path + Text::formatNumDigitPadding( i, 3 ) + ".json";

		std::ifstream ifs( file_path );

		if ( ifs.is_open() )
		{
			rapidjson::IStreamWrapper ifs_wrap( ifs );
			rapidjson::Document lv;
			lv.ParseStream( ifs_wrap );

			if ( lv.IsObject() && lv.HasMember( "name" ) && lv[ "name" ].IsString() )
			{
				level_list_.emplace_back( lv[ "name" ].GetString() );
				++real_level_num_;
			}
			else
			{
				level_list_.emplace_back( "" );
			}

			if ( lv.IsObject() && lv.HasMember( "gem_challenge" ) && lv[ "gem_challenge" ].IsInt() )
			{
				gem_challenge_list_.emplace_back( lv[ "gem_challenge" ].GetInt() );
			}
			else
			{
				gem_challenge_list_.emplace_back( 0 );
			}

			if ( lv.IsObject() && lv.HasMember( "time_challenge" ) && lv[ "time_challenge" ].IsInt() )
			{
				time_challenge_list_.emplace_back( lv[ "time_challenge" ].GetInt() );
			}
			else
			{
				time_challenge_list_.emplace_back( 0 );
			}

			ifs.close();
		}
		else
		{
			level_list_.emplace_back( "" );
			gem_challenge_list_.emplace_back( 0 );
			time_challenge_list_.emplace_back( 0 );
		}

	}
	
	// Debug Test
	//checkLvList();
};

void Level::checkLvList()
{
	for ( unsigned int i = 0; i < level_list_.size(); ++i )
	{
		if ( level_list_.at( i ) != "" )
		{
			std::cout<<Text::formatNumDigitPadding( i, 3 )<<"    "<<level_list_.at( i );
			
			for ( int j = 0; j < 24 - ( int )( level_list_.at( i ).size() ); ++j )
			{
				std::cout<<" ";
			}
			
			std::cout<<Text::formatNumCommas( Text::formatNumDigitPadding( gem_challenge_list_.at( i ), 5 ) )<<"    "<<Clock::timeToString( time_challenge_list_.at( i ) )<<std::endl;
		}
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
				m.sprite( i ) == 450 ||
				m.sprite( i ) == 451 ||
				m.sprite( i ) == 453 ||
				m.sprite( i ) == 454 ||
				m.sprite( i ) == 455 ||
				m.sprite( i ) == 456
			)
			{
				++n;
			}
		}
	}
	
	return n;
};

const std::vector<Map>& Level::allMaps() const
{
	return maps_;
};