#include "audio.hpp"
#include "main.hpp"
#include <fstream>
#include "input.hpp"
#include "level_state.hpp"
#include "main.hpp"
#include "mezun_json.hpp"
#include "overworld_state.hpp"
#include "overworld_menu_state.hpp"
#include "unit.hpp"

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

OverworldState::OverworldState()
:
	GameState( StateID::OVERWORLD_STATE, { "Overworld Red", 2 } ),
	camera_state_ ( CameraState::MOVE_PLAYER ),
	background_animation_timer_ ( 0 ),
	background_animation_frame_ ( 0 ),
	tilemap_ (),
	bg_texture_ (),
	fg_texture_ (),
	water_background_ ( "bg/overworld-water.png", { 0, 0, 400, 224 } ),
	camera_ ( 800, 448 ),
	hero_ ( 300, 300 ),
	inventory_ (),
	objects_ (),
	current_level_ ( -1 )
{};

OverworldState::~OverworldState()
{
	bg_texture_.destroy();
	fg_texture_.destroy();
};

void OverworldState::stateUpdate()
{
	switch ( camera_state_ )
	{
		case ( CameraState::MOVE_PLAYER ):
		{
			testForMenuAction();
			camera_.adjust( hero_.getGraphicsBox() );
			hero_.update( tilemap_, camera_.getBox() );

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

			if ( Input::held( Input::Action::CONFIRM ) )
			{
				if ( current_level_ > -1 )
				{
					Main::changeState( std::make_unique<LevelState> ( current_level_ ) );
				}
			}
		}
		break;

		case ( CameraState::MOVE_CAMERA ):
		{
			if ( Input::pressed( Input::Action::MENU ) )
			{
				camera_state_ = CameraState::CAMERA_MOVES_AUTOMATICALLY_TO_PLAYER;
			}
			else
			{
				camera_.move();
			}
		}
		break;

		case ( CameraState::CAMERA_MOVES_AUTOMATICALLY_TO_PLAYER ):
		{
			if ( camera_.moveAutomaticallyToTarget( hero_.getPosition() ) )
			{
				camera_state_ = CameraState::MOVE_PLAYER;
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
};

void OverworldState::stateRender()
{
	Render::colorCanvas( 2 );
	water_background_.render();
	bg_texture_.render();
	hero_.render( camera_ );
	fg_texture_.render();
	switch ( camera_state_ )
	{
		case ( CameraState::MOVE_PLAYER ):
		{
			inventory_.render();
		}
		break;

		case ( CameraState::MOVE_CAMERA ):
		{
			renderCameraArrows();
		}
		break;

		case ( CameraState::CAMERA_MOVES_AUTOMATICALLY_TO_PLAYER ):
		{
		}
		break;
	}
};

void OverworldState::init()
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
	const int width_blocks = data[ "width" ].GetInt();
	const double width_blocks_d = ( double )( width_blocks );
	tilemap_.map_width = data[ "width" ].GetInt();
	const int width = Unit::BlocksToPixels( tilemap_.map_width );
	const int height = Unit::BlocksToPixels( data[ "height" ].GetInt() );
	bg_texture_.changeSize( width, height );
	fg_texture_.changeSize( width, height );
	std::vector<int> bg_tiles;
	std::vector<int> block_tiles;
	std::vector<int> sprites_tiles;
	bg_tiles.reserve( width & height );

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
							bg_tiles.emplace_back( bg_tile.GetInt() );
						}
					}
				}
				else if ( strcmp( item[ "name" ].GetString(), "Blocks" ) == 0 )
				{
					for ( const auto& block_tile : item[ "data" ].GetArray() )
					{
						if ( block_tile.IsInt() )
						{
							block_tiles.emplace_back( block_tile.GetInt() );
						}
					}
				}
				else if ( strcmp( item[ "name" ].GetString(), "Sprites" ) == 0 )
				{
					for ( const auto& sprites_tile : item[ "data" ].GetArray() )
					{
						if ( sprites_tile.IsInt() )
						{
							sprites_tiles.emplace_back( sprites_tile.GetInt() );
						}
					}
				}
			}
		}
	}

	if ( bg_tiles.size() == 0 )
	{
		throw std::runtime_error( "Missing background layer for o’erworld map." );
	}
	else if ( block_tiles.size() == 0 )
	{
		throw std::runtime_error( "Missing blocks layer for o’erworld map." );
	}
	else if ( sprites_tiles.size() == 0 )
	{
		throw std::runtime_error( "Missing sprite layer for o’erworld map." );
	}

	camera_.setBounds( width, height );
	bg_texture_.init();
	fg_texture_.init();
	sdl2::SDLRect src = { 0, 0, 16, 16 };
	sdl2::SDLRect dest = { 0, 0, 16, 16 };
	bg_texture_.startDrawing();
	for ( int i = 0; i < bg_tiles.size(); ++i )
	{
		const int bg_tile = bg_tiles[ i ] - 1;
		const int x = i % width_blocks;
		const int y = ( int )( std::floor( ( double )( i ) / width_blocks_d ) );
		dest.x = Unit::BlocksToPixels( x );
		dest.y = Unit::BlocksToPixels( y );
		if ( bg_tile > -1 )
		{
			src.x = Unit::BlocksToPixels( bg_tile % 16 );
			src.y = Unit::BlocksToPixels( ( int )( std::floor( ( double )( bg_tile ) / 16.0 ) ) );
			Render::renderObject( "tilesets/ow.png", src, dest );
		}
		const bool is_solid = testIsSolid( bg_tile );
		tilemap_.tiles.emplace_back( is_solid );
		const int sprite_tile = sprites_tiles[ i ] - 1;
		if ( sprite_tile > -1 )
		{
			if ( sprite_tile < 2048 )
			{
				throw std::runtime_error( "Invalid sprite in o’erworld map." );
			}
			if ( sprite_tile == 2048 )
			{
				hero_.setPosition( dest.x + 8, dest.y + 8, camera_.getBox() );
			}
			else if ( sprite_tile > 2063 && sprite_tile < 2063 + ( 16 * 4 ) )
			{
				objects_.insert( std::pair<int, OWObject>( i, OWObject::createLevel( sprite_tile - 2064 ) ) );
			}
		}
	}
	bg_texture_.endDrawing();

	fg_texture_.startDrawing();
	for ( int i = 0; i < block_tiles.size(); ++i )
	{
		const int block_tile = block_tiles[ i ] - 1;
		dest.x = Unit::BlocksToPixels( i % width_blocks );
		dest.y = Unit::BlocksToPixels( ( int )( std::floor( ( double )( i ) / width_blocks_d ) ) );
		if ( block_tile > -1 )
		{
			src.x = Unit::BlocksToPixels( block_tile % 16 );
			src.y = Unit::BlocksToPixels( ( int )( std::floor( ( double )( block_tile ) / 16.0 ) ) );
			Render::renderObject( "tilesets/ow.png", src, dest );
		}
		const bool is_solid = testIsSolid( block_tile );
		if ( !is_solid )
		{
			tilemap_.tiles[ i ] = false;
		}
		else if ( block_tile != -1 )
		{
			tilemap_.tiles[ i ] = true;
		}
	}
	fg_texture_.endDrawing();

	camera_.center( hero_.getPosition() );
	hero_.updateGraphics( camera_.getBox() );

	inventory_.init();

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
			std::make_unique<OverworldMenuState> ( palette() )
		);
	}
};

void OverworldState::backFromPop()
{
	Audio::changeSong( "overworld" );
};

void OverworldState::renderCameraArrows()
{

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