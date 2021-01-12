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
#include "level_list.hpp"
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

static std::vector<int> gem_challenge_list_;
static std::vector<int> time_challenge_list_;
static std::vector<bool> has_secret_goals_;

static std::string code_names_[ Level::NUMBER_OF_LEVELS ] =
{
	""
};

static constexpr char THEMES[ Level::NUMBER_OF_THEMES ][ 9 ] =
{
	"city",
	"domestic",
	"woods",
	"mines",
	"desert",
	"mountain",
	"sky",
	"space",
	"ice",
	"pirate",
	"swamp",
	"muerto",
	"sewer",
	"factory",
	"dungeon",
	"special"
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

const std::u32string& Level::message() const
{
	return message_;
};

int Level::id() const
{
	return id_;
};

const std::string& Level::getCodeName() const
{
	return LevelList::getCodeNameFromID( id_ );
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

void Level::buildCodeNames()
{
	int i = 0;
	for ( int cycle = 1; cycle <= NUMBER_OF_CYCLES; ++cycle )
	{
		for ( int theme_id = 0; theme_id < NUMBER_OF_THEMES; ++theme_id )
		{
			const std::string theme = THEMES[ theme_id ];
			code_names_[ i ] = theme + "-" + std::to_string( cycle );
			++i;
		}
	}
	code_names_[ i++ ] = "final";
	code_names_[ i ] = "bonus";
};

void Level::buildLevelList()
{
	const std::string path = Main::resourcePath() + "levels" + Main::pathDivider();

	if ( !mezun::checkDirectory( path ) )
	{
		throw mezun::CantLoadLevelNames();
	}

	const std::vector<std::string> list = LevelList::getListOfCodeNames( path );

	for ( int i = 0; i < NUMBER_OF_LEVELS; ++i )
	{
		const std::string local = ( i == NUMBER_OF_LEVELS - 1 ) ? "bonus" : ( i == NUMBER_OF_LEVELS - 2 ) ? "final" : code_names_[ i ];
		const std::string file_path = path + local + ".json";
		std::ifstream ifs( file_path );

		if ( ifs.is_open() )
		{
			rapidjson::IStreamWrapper ifs_wrap( ifs );
			rapidjson::Document lv;
			lv.ParseStream( ifs_wrap );

			if ( lv.IsObject() )
			{
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
				Main::changeState( std::unique_ptr<GameState> ( WMessageState::generateErrorMessage( mezun::charToChar32String( std::string( "Level “" + local + "”’s JSON file in assets/levels has been corrupted. Please redownload game." ).c_str() ), WMessageState::Type::POP, nullptr ) ) );
			}

			ifs.close();
		}
		else
		{
			gem_challenge_list_.emplace_back( 0 );
			time_challenge_list_.emplace_back( 0 );
			has_secret_goals_.emplace_back( false );
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