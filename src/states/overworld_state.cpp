#include "audio.hpp"
#include "main.hpp"
#include <fstream>
#include "input.hpp"
#include "inventory.hpp"
#include "level_state.hpp"
#include "main.hpp"
#include "mezun_json.hpp"
#include "overworld_state.hpp"
#include "overworld_menu_state.hpp"
#include "unit.hpp"

static std::vector<int> bg_tiles_ = {};
static std::vector<int> fg_tiles_ = {};
static std::vector<int> sprites_tiles_ = {};
static int width_blocks_ = 0;
static int height_blocks_ = 0;

static constexpr int NUMBER_OF_SOLID_TILES = 22;
static constexpr int SOLID_TILES[ NUMBER_OF_SOLID_TILES ] =
{
	-1, 21, 23, 24, 25, 26, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47
};

static constexpr bool testIsSolid( int tile )
{
	for ( int i = 0; i < NUMBER_OF_SOLID_TILES; ++i )
	{
		if ( tile == SOLID_TILES[ i ] )
		{
			return true;
		}
	}
	return false;
};

OverworldState::OverworldState( int previous_level, bool show_event, bool new_game )
:
	GameState( StateID::OVERWORLD_STATE, { "Overworld Red", 2 } ),
	state_ ( ( show_event ) ? OWState::CAMERA_MOVES_TO_EVENT : OWState::MOVE_PLAYER ),
	background_animation_timer_ ( 0 ),
	background_animation_frame_ ( 0 ),
	current_level_ ( -1 ),
	previous_level_ ( previous_level ),
	objects_ (),
	tilemap_ (),
	bg_texture_ (),
	fg_texture_ (),
	water_background_ ( "bg/overworld-water.png", { 0, 0, 400, 224 } ),
	camera_ ( 800, 448 ),
	event_ (),
	camera_arrows_ (),
	hero_ ( 300, 300 ),
	level_tile_graphics_ (),
	inventory_ ()
{
	if ( new_game )
	{
		bg_tiles_.clear();
		fg_tiles_.clear();
		sprites_tiles_.clear();
	}
};

OverworldState::~OverworldState()
{
	bg_texture_.destroy();
	fg_texture_.destroy();
};

void OverworldState::stateUpdate()
{
	switch ( state_ )
	{
		case ( OWState::MOVE_PLAYER ):
		{
			testForMenuAction();
			camera_.adjust( hero_.getGraphicsBox() );
			hero_.update( tilemap_, camera_.getBox() );

			const int level_momento = current_level_;
			current_level_ = -1;
			const DPoint& hero_position = hero_.getPosition();
			const int tile_x = ( int )( std::floor( ( hero_position.x ) / 16.0 ) );
			const int tile_y = ( int )( std::floor( ( hero_position.y ) / 16.0 ) );
			const int tile = tile_y * tilemap_.map_width + tile_x;
			const auto& seek = objects_.find( tile );
			if ( seek != objects_.end() )
			{
				current_level_ = seek->second.value.level;
			}
			inventory_.update( current_level_ );

			if ( Input::pressed( Input::Action::CONFIRM ) )
			{
				if ( current_level_ > -1 )
				{
					Main::changeState( std::make_unique<LevelState> ( current_level_ ) );
				}
			}
			else
			{
				if ( current_level_ != -1 && current_level_ != level_momento )
				{
					Audio::playSound( Audio::SoundType::SELECT );
				}
			}
		}
		break;

		case ( OWState::MOVE_CAMERA ):
		{
			if ( Input::pressed( Input::Action::MENU ) )
			{
				state_ = OWState::CAMERA_MOVES_AUTOMATICALLY_TO_PLAYER;
			}
			else
			{
				camera_.move();
				hero_.updateGraphics( camera_.getBox() );
				camera_arrows_.update( camera_.getBox() );
			}
		}
		break;

		case ( OWState::CAMERA_MOVES_AUTOMATICALLY_TO_PLAYER ):
		{
			hero_.updateGraphics( camera_.getBox() );
			if ( camera_.moveAutomaticallyToTarget( hero_.getPosition() ) )
			{
				state_ = OWState::MOVE_PLAYER;
			}
		}
		break;

		case ( OWState::EVENT ):
		{
			const OWEvent::MessageBack message = event_.update( bg_tiles_, fg_tiles_ );
			switch ( message )
			{
				case ( OWEvent::MessageBack::BOTH_TILES_CHANGED ):
				{
					generateMapTextures();
				}
				break;

				case ( OWEvent::MessageBack::BG_TILES_CHANGED ):
				{
					generateBGMapTexture();
				}
				break;

				case ( OWEvent::MessageBack::FG_TILES_CHANGED ):
				{
					generateFGMapTexture();
				}
				break;

				case ( OWEvent::MessageBack::EVENT_OVER ):
				{
					state_ = OWState::CAMERA_MOVES_AUTOMATICALLY_TO_PLAYER;
				}
				break;
			}
		}
		break;

		case ( OWState::CAMERA_MOVES_TO_EVENT ):
		{
			hero_.updateGraphics( camera_.getBox() );
			if ( camera_.moveAutomaticallyToTarget( event_.getTargetPosition(), 4 ) )
			{
				state_ = OWState::EVENT;
			}
		}
		break;
	}

	bg_texture_.setX( -camera_.x() );
	bg_texture_.setY( -camera_.y() );
	fg_texture_.setX( -camera_.x() );
	fg_texture_.setY( -camera_.y() );
	updateBackgroundAnimation();
	updateBackgroundPosition();
	level_tile_graphics_.update( camera_.getBox() );
};

void OverworldState::stateRender()
{
	Render::colorCanvas( 2 );
	water_background_.render();
	bg_texture_.render();
	level_tile_graphics_.render();
	hero_.render( camera_ );
	fg_texture_.render();
	switch ( state_ )
	{
		case ( OWState::MOVE_PLAYER ):
		{
			inventory_.render();
		}
		break;

		case ( OWState::MOVE_CAMERA ):
		{
			camera_arrows_.render();
		}
		break;

		case ( OWState::CAMERA_MOVES_AUTOMATICALLY_TO_PLAYER ):
		{
		}
		break;

		case ( OWState::EVENT ):
		{
		}
		break;
	}
};

void OverworldState::init()
{
	if ( bg_tiles_.size() == 0 )
	{
		generateMap();
	}

	tilemap_.map_width = width_blocks_;
	const int width = Unit::BlocksToPixels( width_blocks_ );
	const int height = Unit::BlocksToPixels( height_blocks_ );
	bg_texture_.changeSize( width, height );
	fg_texture_.changeSize( width, height );
	bg_tiles_.reserve( width * height );
	fg_tiles_.reserve( width * height );

	camera_.setBounds( width, height );
	bg_texture_.init();
	fg_texture_.init();
	tilemap_.tiles.reserve( width * height );

	generateSprites();
	generateMapTextures();

	camera_.center( hero_.getPosition() );
	hero_.updateGraphics( camera_.getBox() );

	inventory_.init();

	if ( state_ == OWState::CAMERA_MOVES_TO_EVENT )
	{
		event_.init( Inventory::currentLevel(), width_blocks_ );
	}

	Audio::changeSong( "overworld" );
};

void loadMap( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data )
{

};

void OverworldState::testForMenuAction()
{
	if ( Input::pressed( Input::Action::MENU ) )
	{
		Main::pushState
		(
			std::make_unique<OverworldMenuState> ( palette(), &state_ )
		);
	}
};

void OverworldState::backFromPop()
{
	Audio::changeSong( "overworld" );
	if ( state_ == OWState::MOVE_CAMERA )
	{
		Audio::playSound( Audio::SoundType::PAUSE );
	}
};

void OverworldState::updateBackgroundAnimation()
{
	if ( background_animation_timer_ == 7 )
	{
		background_animation_timer_ = 0;
		if ( background_animation_frame_ == 15 )
		{
			background_animation_frame_ = 0;
		}
		else
		{
			++background_animation_frame_;
		}
	}
	else
	{
		++background_animation_timer_;
	}
};

void OverworldState::updateBackgroundPosition()
{
	water_background_.src_.x = ( camera_.x() + background_animation_frame_ ) % 16;
	water_background_.src_.y = camera_.y() % 16;
};

void OverworldState::generateSprites()
{
	sdl2::SDLRect src = { 0, 0, 16, 16 };
	sdl2::SDLRect dest = { 0, 0, 16, 16 };
	for ( int i = 0; i < sprites_tiles_.size(); ++i )
	{
		const int sprite_tile = sprites_tiles_[ i ] - 1;
		dest.x = Unit::BlocksToPixels( i % width_blocks_ );
		dest.y = Unit::BlocksToPixels( ( int )( std::floor( ( double )( i ) / ( double )( width_blocks_ ) ) ) );
		if ( sprite_tile > -1 )
		{
			if ( sprite_tile < 2048 )
			{
				throw std::runtime_error( "Invalid sprite in o’erworld map." );
			}
			if ( sprite_tile == 2048 )
			{
				if ( previous_level_ == -1 )
				{
					hero_.setPosition( dest.x + 8, dest.y + 8, camera_.getBox() );
				}
			}
			else if ( sprite_tile > 2063 && sprite_tile < 2063 + ( 16 * 4 ) )
			{
				const int level_id = sprite_tile - 2064;
				if ( previous_level_ == level_id )
				{
					hero_.setPosition( dest.x + 8, dest.y + 8, camera_.getBox() );
				}
				objects_.insert( std::pair<int, OWObject>( i, OWObject::createLevel( level_id ) ) );
				level_tile_graphics_.add( dest );
			}
		}
	}
};

void OverworldState::generateMapTextures()
{
	generateBGMapTexture();
	generateFGMapTexture();
};

void OverworldState::generateBGMapTexture()
{
	sdl2::SDLRect src = { 0, 0, 16, 16 };
	sdl2::SDLRect dest = { 0, 0, 16, 16 };
	bg_texture_.startDrawing();
	Render::clearScreenTransparency();
	for ( int i = 0; i < bg_tiles_.size(); ++i )
	{
		const int bg_tile = bg_tiles_[ i ] - 1;
		const int x = i % width_blocks_;
		const int y = ( int )( std::floor( ( double )( i ) / ( double )( width_blocks_ ) ) );
		dest.x = Unit::BlocksToPixels( x );
		dest.y = Unit::BlocksToPixels( y );
		if ( bg_tile > -1 )
		{
			src.x = Unit::BlocksToPixels( bg_tile % 16 );
			src.y = Unit::BlocksToPixels( ( int )( std::floor( ( double )( bg_tile ) / 16.0 ) ) );
			Render::renderObject( "tilesets/ow.png", src, dest );
		}
		const bool is_solid = testIsSolid( bg_tile );
		tilemap_.tiles[ i ] = is_solid;
	}
	bg_texture_.endDrawing();
};

void OverworldState::generateFGMapTexture()
{
	sdl2::SDLRect src = { 0, 0, 16, 16 };
	sdl2::SDLRect dest = { 0, 0, 16, 16 };
	fg_texture_.startDrawing();
	Render::clearScreenTransparency();
	for ( int i = 0; i < fg_tiles_.size(); ++i )
	{
		const int fg_tile = fg_tiles_[ i ] - 1;
		dest.x = Unit::BlocksToPixels( i % width_blocks_ );
		dest.y = Unit::BlocksToPixels( ( int )( std::floor( ( double )( i ) / ( double )( width_blocks_ ) ) ) );
		if ( fg_tile > -1 )
		{
			src.x = Unit::BlocksToPixels( fg_tile % 16 );
			src.y = Unit::BlocksToPixels( ( int )( std::floor( ( double )( fg_tile ) / 16.0 ) ) );
			Render::renderObject( "tilesets/ow.png", src, dest );
		}
		const bool is_solid = testIsSolid( fg_tile );
		if ( !is_solid )
		{
			tilemap_.tiles[ i ] = false;
		}
		else if ( fg_tile != -1 )
		{
			tilemap_.tiles[ i ] = true;
		}
	}
	fg_texture_.endDrawing();
};

void OverworldState::generateMap()
{
	const std::string path = Main::resourcePath() + "maps/land-ow.json";
	std::ifstream ifs( path );
	if( !ifs.is_open() )
	{
		throw std::runtime_error( "The o’erworld map data is missing. Please redownload this game & try ’gain." );
	}
	rapidjson::IStreamWrapper ifs_wrap( ifs );
    rapidjson::Document document;
    document.ParseStream( ifs_wrap );
	ifs.close();

    if ( !document.IsObject() )
    {
		throw std::runtime_error( "The o’erworld map data is broken. Please redownload this game & try ’gain." );
    }

    const auto data = document.GetObject();
	if
	(
		!data.HasMember( "width" )  ||
		!data[ "width" ].IsInt()    ||
		!data.HasMember( "height" ) ||
		!data[ "height" ].IsInt()   ||
		!data.HasMember( "layers" ) ||
		!data[ "layers" ].IsArray()
	)
	{
		throw std::runtime_error( "The o’erworld map data is broken. Please redownload this game & try ’gain." );
	}
	width_blocks_ = data[ "width" ].GetInt();
	height_blocks_ = data[ "height" ].GetInt();

	for ( const auto& i : data[ "layers" ].GetArray() )
	{
		if ( i.IsObject() )
		{
			const auto& item = i.GetObject();
			const bool has_name = item.HasMember( "name" );
			const bool name_is_string = item[ "name" ].IsString();
			const bool has_data = item.HasMember( "data" );
			const bool data_is_array = item[ "data" ].IsArray();

			if ( has_name && name_is_string && has_data && data_is_array )
			{
				const int name_is_bg = strcmp( item[ "name" ].GetString(), "BG" );
				if ( name_is_bg == 0 )
				{
					for ( const auto& bg_tile : item[ "data" ].GetArray() )
					{
						if ( bg_tile.IsInt() )
						{
							bg_tiles_.emplace_back( bg_tile.GetInt() );
						}
					}
				}
				else if ( strcmp( item[ "name" ].GetString(), "Blocks" ) == 0 )
				{
					for ( const auto& fg_tile : item[ "data" ].GetArray() )
					{
						if ( fg_tile.IsInt() )
						{
							fg_tiles_.emplace_back( fg_tile.GetInt() );
						}
					}
				}
				else if ( strcmp( item[ "name" ].GetString(), "Sprites" ) == 0 )
				{
					for ( const auto& sprites_tile : item[ "data" ].GetArray() )
					{
						if ( sprites_tile.IsInt() )
						{
							sprites_tiles_.emplace_back( sprites_tile.GetInt() );
						}
					}
				}
			}
		}
	}

	if ( bg_tiles_.size() == 0 )
	{
		throw std::runtime_error( "Missing background layer for o’erworld map." );
	}
	else if ( fg_tiles_.size() == 0 )
	{
		throw std::runtime_error( "Missing blocks layer for o’erworld map." );
	}
	else if ( sprites_tiles_.size() == 0 )
	{
		throw std::runtime_error( "Missing sprite layer for o’erworld map." );
	}

	for ( int i = 0; i < Level::NUMBER_OF_LEVELS; ++i )
	{
		if ( Inventory::victory( i ) )
		{
			OWEvent event;
			event.init( i, width_blocks_ );
			event.changeAllTiles( bg_tiles_, fg_tiles_ );
		}
	}
};