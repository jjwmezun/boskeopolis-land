#include "collision.h"
#include "game.h"
#include <fstream>
#include "level_state.h"
#include "mezun_math.h"
#include "overworld_state.h"
#include "overworld_menu_state.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "unit.h"

OverworldState::OverworldState()
:
	GameState( StateID::OVERWORLD_STATE, { "Overworld Red", 2 } ),
	camera_ (),
	hero_ ( Unit::BlocksToPixels( 66 ), Unit::BlocksToPixels( 152 ) ),
	inventory_ (),
	events_ (),
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
	)
{
	mapData();
	camera_.center( hero_.x(), hero_.y(), hero_.W, hero_.H, Unit::BlocksToPixels( map_width_ ), Unit::BlocksToPixels( map_height_ ) );
};

OverworldState::OverworldState( const EventSystem& events, const Inventory& inventory, int level )
:
	GameState( StateID::OVERWORLD_STATE, lvPal( level ) ),
	camera_ (),
	hero_ ( Unit::BlocksToPixels( 66 ), Unit::BlocksToPixels( 152 ) ),
	inventory_ ( inventory ),
	events_ ( events ),
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
	)
{
	mapData();
	for ( auto& l : level_tiles_ )
	{
		if ( l.lv() == level )
		{
			hero_.placeOnLv( l );
		}
	}
	camera_.center( hero_.x(), hero_.y(), hero_.W, hero_.H, Unit::BlocksToPixels( map_width_ ), Unit::BlocksToPixels( map_height_ ) );
	mapEvents();
};

OverworldState::~OverworldState() {};

void OverworldState::update( Game& game, const Input& input, Graphics& graphics )
{
	level_selection_ = NULL;
	water_gfx_.update();
	hero_.update( input );
	camera_.adjust( hero_.x(), hero_.y(), hero_.W, hero_.H, Unit::BlocksToPixels( map_width_ ), Unit::BlocksToPixels( map_height_ ) );
	inventory_.update( input, level_selection_ );
	
	interactions( graphics );
	lv_gfx_.update();
	
	menu( game, input );
	if ( level_selection_ > 0 )
	{
		if ( input.pressed( Input::Action::CONFIRM ) )
		{
			game.changeState( std::unique_ptr<GameState> ( new LevelState( events_, inventory_.inventory(), level_selection_, game ) ) );
		}
	}
};

void OverworldState::stateRender( Graphics& graphics )
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
				tiles_.at( i ).render( graphics, camera_ );
			}
			else
			{
				break;
			}
		}
	}

	for ( auto& l : level_tiles_ )
	{
		l.render( graphics, camera_ );
	}

	hero_.render( graphics, camera_ );
	inventory_.render( graphics, level_selection_ );
};

void OverworldState::init( Game& game, Graphics& graphics ) {};

void OverworldState::mapData()
{
	const std::string MAP_PATH = Game::resourcePath() + "maps" + Game::pathDivider() + "land-ow.json";

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
						
						if ( l >= 32 && l < 32 + 40 )
						{
							level_tiles_.emplace_back( list[ j ].GetInt() - 33, Unit::BlocksToPixels( mezun::xOfN( j, map_width_ ) ), Unit::BlocksToPixels( mezun::yOfN( j, map_width_ ) ), lv_gfx_ );
						}
					}
					else if ( i == PAL_CHANGE_INDEX )
					{
						const int p = list[ j ].GetInt();

						if ( p >= 32 && p < 32 + 40 )
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

void OverworldState::interactions( Graphics& graphics )
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
			}
			
			newPalette( graphics, { pal, 2 } );
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
	for ( int i = 0; i < Level::NUM_O_LEVELS; ++i )
	{
		if ( inventory_.inventory().victory( i ) )
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
			tiles_.at( mezun::nOfXY( 59, 150, map_width_ ) ).type_ = 5;
		break;

		case ( 2 ):
			tiles_.at( mezun::nOfXY( 36, 139, map_width_ ) ).type_ = 5;
		break;
	}
};

void OverworldState::menu( Game& game, const Input& input )
{
	if ( input.pressed( Input::Action::MENU ) )
	{
		game.pushState
		(
			std::unique_ptr<GameState>
			(
				new OverworldMenuState()
			)
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
			pal = "Overworld Yellow";
		break;
	}
	
	return { pal, 2 };
};