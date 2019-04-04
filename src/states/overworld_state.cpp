#include "audio.hpp"
#include "collision.hpp"
#include "main.hpp"
#include <fstream>
#include "input.hpp"
#include "inventory.hpp"
#include "level_select_state.hpp"
#include "level_state.hpp"
#include "message_state.hpp"
#include "mezun_math.hpp"
#include "overworld_state.hpp"
#include "overworld_menu_state.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "unit.hpp"
#include "mezun_exceptions.hpp"
#include <iostream>

static constexpr int SMOOTH_MOVEMENT_PADDING = 4;

OverworldState::OverworldState()
:
	GameState( StateID::OVERWORLD_STATE, { "Overworld Red", 2 } ),
	camera_ (),
	hero_ ( Unit::BlocksToPixels( 66 ), Unit::BlocksToPixels( 152 ) ),
	inventory_screen_ (),
	tiles_ (),
	level_tiles_ (),
	pal_change_tiles_ (),
	level_selection_ ( NULL ),
	water_gfx_
	(
		"tilesets/ow.png",
		{
			std::make_pair( 0, Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 1 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 2 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 3 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 4 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 5 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 6 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 7 ), Unit::BlocksToPixels( 6 ) )
		}
	),
	lv_gfx_
	(
		"tilesets/ow.png",
		{
			std::make_pair( Unit::BlocksToPixels( 8 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 9 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 10 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 11 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 12 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 13 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 13 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 12 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 11 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 10 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 9 ), Unit::BlocksToPixels( 6 ) ),
			std::make_pair( Unit::BlocksToPixels( 8 ), Unit::BlocksToPixels( 6 ) )
		}
	),
	go_to_list_ ( false ),
	camera_mode_ ( false ),
	camera_trans_ ( false )
{
	mapData();

	camera_.center( hero_.x(), hero_.y(), hero_.W, hero_.H, Unit::BlocksToPixels( map_width_ ), Unit::BlocksToPixels( map_height_ ) );
	mapEvents();
};

OverworldState::~OverworldState()
{
};

void OverworldState::stateUpdate()
{
	if ( camera_mode_ )
	{
		if ( Input::pressed( Input::Action::MENU ) )
		{
			camera_mode_ = false;
			camera_trans_ = true;
		}

		camera_.move( Unit::BlocksToPixels( map_width_ ), Unit::BlocksToPixels( map_height_ ) );
	}
	else if ( camera_trans_ )
	{
		if ( camera_.backToHero( hero_.x(), hero_.y(), hero_.W, hero_.H, Unit::BlocksToPixels( map_width_ ), Unit::BlocksToPixels( map_height_ ) ) )
		{
			camera_trans_ = false;
		}
	}
	else
	{
		level_selection_ = NULL;
		water_gfx_.update();
		hero_.update();
		camera_.adjust( hero_.x(), hero_.y(), hero_.W, hero_.H, Unit::BlocksToPixels( map_width_ ), Unit::BlocksToPixels( map_height_ ) );
		interactions();
		lv_gfx_.update();

		inventory_screen_.update( level_selection_ );

		menu();
		if ( level_selection_ > 0 )
		{
			try
			{
				if ( Input::pressed( Input::Action::CONFIRM ) )
				{
					Main::changeState( std::unique_ptr<GameState> ( new LevelState( level_selection_ ) ) );
					Audio::playSound( Audio::SoundType::CONFIRM );
				}
			}
			catch( const mezun::MissingLevel e )
			{
				Main::pushState
				(
					std::unique_ptr<MessageState> ( MessageState::errorMessage( e.what() ) ),
					true
				);
			}
			catch( const mezun::BrokenLevelFile e )
			{
				Main::pushState
				(
					std::unique_ptr<MessageState> ( MessageState::errorMessage( e.what() ) ),
					true
				);
			}
		}
	}
};

void OverworldState::stateRender()
{
	const int y_init = std::max( Unit::PixelsToBlocks( camera_.y() ) - 1, 0 );
	const int x_init = std::max( Unit::PixelsToBlocks( camera_.x() ) - 1, 0 );
	const int y_end  = std::min( y_init + camera_.W + 2, map_height_ );
	const int x_end  = std::min( x_init + camera_.H + 2, map_width_ );

	for ( int y = y_init; y < y_end; ++y )
	{
		for ( int x = x_init; x < x_end; ++x )
		{
			const int i = mezun::nOfXY( x, y, map_width_ );

			if ( i < tiles_.size() )
			{
				tiles_.at( i ).render( camera_ );
			}
			else
			{
				break;
			}
		}
	}

	for ( auto& l : level_tiles_ )
	{
		l.render( camera_ );
	}

	hero_.render( camera_ );
	inventory_screen_.render( level_selection_ );

	if ( camera_mode_ )
	{
		renderCameraArrows();
	}
};

void OverworldState::init()
{
	int level = Inventory::currentLevel();
	if ( level != NULL )
	{
		for ( auto& l : level_tiles_ )
		{
			if ( l.lv() == level )
			{
				hero_.placeOnLv( l );
				newPalette( lvPal( level ) );
			}
		}
	}
	Audio::changeSong( "overworld" );
};

void OverworldState::mapData()
{
	const std::string MAP_PATH = Main::resourcePath() + "maps" + Main::pathDivider() + "land-ow.json";

	std::ifstream map_stream( MAP_PATH );

	assert( map_stream.is_open() );

	rapidjson::IStreamWrapper map_stream_wrapper( map_stream );
	rapidjson::Document map_data;
	map_data.ParseStream( map_stream_wrapper );

	assert( map_data.IsObject() );

	// Get Map Sizes
	//=============================================

	assert( map_data.HasMember( "width" ) );
	assert( map_data[ "width" ].IsInt() );
	map_width_ = map_data[ "width" ].GetInt();

	assert( map_data.HasMember( "height" ) );
	assert( map_data[ "height" ].IsInt() );
	map_height_ = map_data[ "height" ].GetInt();


	// Get Layers
	//=============================================

	assert( map_data.HasMember( "layers" ) );
	assert( map_data[ "layers" ].IsArray() );

	constexpr int BLOCKS_INDEX = 0;
	constexpr int LEVELS_INDEX = 1;
	constexpr int PAL_CHANGE_INDEX = 2;

	int i = 0;
	for ( auto& v : map_data[ "layers" ].GetArray() )
	{
		if ( v.HasMember("data") )
		{
			if ( v[ "data" ].IsArray() )
			{
				const auto list = v[ "data" ].GetArray();

				for ( int j = 0; j < list.Size(); ++j )
				{
					if ( i == BLOCKS_INDEX )
					{
						tiles_.emplace_back( list[ j ].GetInt() - 1, Unit::BlocksToPixels( mezun::xOfN( j, map_width_ ) ), Unit::BlocksToPixels( mezun::yOfN( j, map_width_ ) ), water_gfx_ );
					}
					else if ( i == LEVELS_INDEX )
					{
						const int l = list[ j ].GetInt();

						if ( l >= 32 && l < 32 + 64 )
						{
							level_tiles_.emplace_back( list[ j ].GetInt() - 33, Unit::BlocksToPixels( mezun::xOfN( j, map_width_ ) ), Unit::BlocksToPixels( mezun::yOfN( j, map_width_ ) ), lv_gfx_ );
						}
					}
					else if ( i == PAL_CHANGE_INDEX )
					{
						const int p = list[ j ].GetInt();

						if ( p >= 32 && p < 32 + 64 )
						{
							pal_change_tiles_.emplace_back( list[ j ].GetInt() - 33, Unit::BlocksToPixels( mezun::xOfN( j, map_width_ ) ), Unit::BlocksToPixels( mezun::yOfN( j, map_width_ ) ) );
						}
					}
					else
					{
						break;
					}
				}
			}
		}
		++i;
	}
};

void OverworldState::interactions()
{
	const int y_init = std::max( Unit::PixelsToBlocks( hero_.y() ) - 1, 0 );
	const int x_init = std::max( Unit::PixelsToBlocks( hero_.x() ) - 1, 0 );
	const int y_end  = std::min( Unit::PixelsToBlocks( hero_.bottom() ) + 2, map_height_ );
	const int x_end  = std::min( Unit::PixelsToBlocks( hero_.right() ) + 2, map_width_ );

	for ( int y = y_init; y < y_end; ++y )
	{
		for ( int x = x_init; x < x_end; ++x )
		{
			const int i = mezun::nOfXY( x, y, map_width_ );

			if ( i < tiles_.size() )
			{
				tiles_.at( i ).interact( hero_, testCollision( hero_.coords(), tiles_.at( i ).coords() ) );
			}
			else
			{
				break;
			}
		}
	}

	for ( auto& l : level_tiles_ )
	{
		if ( testCollision( hero_.coords(), l.coords() ).collideAny() )
		{
			level_selection_ = l.lv();
		}
	}

	for ( auto& p : pal_change_tiles_ )
	{
		if ( testCollision( hero_.coords(), p.coords() ).collideAny() )
		{
			std::string pal = "Overworld Red";

			switch ( p.type )
			{
				case ( 0 ):
				break;

				case ( 1 ):
					pal = "Overworld Green";
				break;

				case ( 2 ):
					pal = "Overworld Yellow";
				break;

				case ( 3 ):
					pal = "Overworld Blue";
				break;

				case ( 4 ):
					pal = "Overworld Gray";
				break;
			}

			newPalette( { pal, 2 } );
		}
	}
};

Collision OverworldState::testCollision( const sdl2::SDLRect& lhs, const sdl2::SDLRect& rhs )
{
	int overlap_x_left   = 0;
	int overlap_x_right  = 0;
	int overlap_y_top    = 0;
	int overlap_y_bottom = 0;

	if
	(
		// Allows sprite to still move vertically, e'en if colliding with a block horizontally.
		lhs.x + SMOOTH_MOVEMENT_PADDING < rhs.right() &&
		lhs.right() - SMOOTH_MOVEMENT_PADDING  > rhs.x &&
		lhs.y < rhs.bottom() &&
		lhs.bottom() > rhs.y
	)
	{
		if ( lhs.centerHeight() > rhs.centerHeight() )
			overlap_y_top = rhs.bottom() - lhs.y;
		else
			overlap_y_bottom = lhs.bottom() - rhs.y;
	}

	if
	(
		lhs.x < rhs.right() &&
		lhs.right() > rhs.x &&
		// Allows sprite to still move horizontally, e'en if colliding with a block vertically.
		lhs.y + SMOOTH_MOVEMENT_PADDING < rhs.bottom() &&
		lhs.bottom() - SMOOTH_MOVEMENT_PADDING > rhs.y
	)
	{
		if ( lhs.centerWidth() > rhs.centerWidth() )
			overlap_x_left = rhs.right() - lhs.x;
		else
			overlap_x_right = lhs.right() - rhs.x;
	}

	return Collision( overlap_x_left, overlap_x_right, overlap_y_top, overlap_y_bottom );
};

void OverworldState::mapEvents()
{
	for ( int i = 0; i < Level::MAX; ++i )
	{
		if ( Inventory::victory( i ) )
		{
			eventByID( i );
		}
	}
};

void OverworldState::eventByID( int id )
{
	switch ( id )
	{
		case ( 1 ):
			tiles_.at( mezun::nOfXY( 59, 150, map_width_ ) ).changeType( 5, water_gfx_ );
		break;

		case ( 2 ):
			tiles_.at( mezun::nOfXY( 36, 139, map_width_ ) ).changeType( 5, water_gfx_ );
		break;

		case ( 3 ):
			for ( int i = 107; i < 121; ++i )
			{
				tiles_.at( mezun::nOfXY( 21, i, map_width_ ) ).changeType( 5, water_gfx_ );
			}
		break;

		case ( 4 ):
			for ( int i = 53; i < 71; ++i )
			{
				tiles_.at( mezun::nOfXY( 24, i, map_width_ ) ).changeType( 26, water_gfx_ );
			}
		break;

		case ( 5 ):
			for ( int i = 36; i < 42; ++i )
			{
				tiles_.at( mezun::nOfXY( 29, i, map_width_ ) ).changeType( 26, water_gfx_ );
			}
			for ( int i = 26; i < 37; ++i )
			{
				tiles_.at( mezun::nOfXY( 39, i, map_width_ ) ).changeType( 26, water_gfx_ );
			}
			for ( int i = 30; i < 39; ++i )
			{
				tiles_.at( mezun::nOfXY( i, 36, map_width_ ) ).changeType( 26, water_gfx_ );
			}
			for ( int i = 40; i < 55; ++i )
			{
				tiles_.at( mezun::nOfXY( i, 26, map_width_ ) ).changeType( 26, water_gfx_ );
			}
		break;

		case ( 6 ):
			for ( int i = 85; i < 95; ++i )
			{
				tiles_.at( mezun::nOfXY( i, 23, map_width_ ) ).changeType( 5, water_gfx_ );
			}
		break;

		case ( 7 ):
			for ( int i = 108; i < 125; ++i )
			{
				tiles_.at( mezun::nOfXY( i, 28, map_width_ ) ).changeType( 5, water_gfx_ );
			}
			for ( int i = 28; i < 43; ++i )
			{
				tiles_.at( mezun::nOfXY( 124, i, map_width_ ) ).changeType( 5, water_gfx_ );
			}
			for ( int i = 124; i < 149; ++i )
			{
				tiles_.at( mezun::nOfXY( i, 42, map_width_ ) ).changeType( 5, water_gfx_ );
			}
			for ( int i = 42; i < 53; ++i )
			{
				tiles_.at( mezun::nOfXY( 148, i, map_width_ ) ).changeType( 5, water_gfx_ );
			}
		break;
	}
};

void OverworldState::menu()
{
	if ( go_to_list_ )
	{
		go_to_list_ = false;

		Main::pushState
		(
			std::make_unique<LevelSelectState> ( level_selection_ ),
			true
		);
	}
	else if ( Input::pressed( Input::Action::MENU ) )
	{
		Main::pushState
		(
			std::make_unique<OverworldMenuState> ( go_to_list_, camera_mode_, palette() )
		);
	}
};

Palette OverworldState::lvPal( int id )
{
	std::string pal = "Overworld Red";

	switch ( id )
	{
		case ( 2 ):
			pal = "Overworld Green";
		break;

		case ( 3 ):
		case ( 4 ):
			pal = "Overworld Yellow";
		break;

		case ( 5 ):
		case ( 6 ):
			pal = "Overworld Blue";
		break;

		case ( 7 ):
		case ( 8 ):
			pal = "Overworld Gray";
		break;
	}

	return { pal, 2 };
};

void OverworldState::renderCameraArrows()
{
	Render::renderObject( "tilesets/ow.png", { 0, Unit::BlocksToPixels( 7 ), 16, 16 }, { Unit::WINDOW_WIDTH_PIXELS / 2 - 8, 16, 16, 16 }, SDL_FLIP_NONE, 0.0 );
	Render::renderObject( "tilesets/ow.png", { 0, Unit::BlocksToPixels( 7 ), 16, 16 }, { Unit::WINDOW_WIDTH_PIXELS - 32, camera_.H / 2 - 8, 16, 16 }, SDL_FLIP_NONE, 90.0 );
	Render::renderObject( "tilesets/ow.png", { 0, Unit::BlocksToPixels( 7 ), 16, 16 }, { Unit::WINDOW_WIDTH_PIXELS / 2 - 8, camera_.H - 32, 16, 16 }, SDL_FLIP_VERTICAL, 0.0 );
	Render::renderObject( "tilesets/ow.png", { 0, Unit::BlocksToPixels( 7 ), 16, 16 }, { 16, camera_.H / 2 - 8, 16, 16 }, SDL_FLIP_NONE, 270.0 );
};

void OverworldState::backFromPop()
{
	Audio::changeSong( "overworld" );
};
