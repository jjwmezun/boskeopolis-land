#include "audio.hpp"
#include "main.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include <fstream>
#include "overworld_state.hpp"
#include "render.hpp"
#include "title_state.hpp"

static constexpr int LOGO_WIDTH = 367;
static constexpr int LOGO_HEIGHT = 38;
static constexpr int LOGO_Y = 16;
static constexpr int CREATED_BY_Y = LOGO_HEIGHT + LOGO_Y + 8;
static constexpr int CREATED_BY_HEIGHT = 8;
static constexpr int OPTIONS_TOP_PADDING = 16;
static constexpr int OPTIONS_TOP_Y = CREATED_BY_Y + CREATED_BY_HEIGHT + OPTIONS_TOP_PADDING;
static constexpr int OPTION_WIDTH_MINIBLOCKS = 12;
static constexpr int OPTION_HEIGHT_MINIBLOCKS = 3;
static constexpr int OPTION_WIDTH_PIXELS = Unit::MiniBlocksToPixels( OPTION_WIDTH_MINIBLOCKS );
static constexpr int OPTION_HEIGHT_PIXELS = Unit::MiniBlocksToPixels( OPTION_HEIGHT_MINIBLOCKS );
static constexpr int OPTION_PADDING = Unit::PIXELS_PER_MINIBLOCK;
static constexpr int OPTIONS_X = ( Unit::WINDOW_WIDTH_PIXELS - OPTION_WIDTH_PIXELS ) / 2;
static constexpr int OPTION_BG_COLOR = 5;
static constexpr int OPTION_HIGHLIGHT_BG_COLOR = 1;
static constexpr int SHADOW_LENGTH = 2;
static constexpr int SHADOW_COLOR = 6;
static constexpr int LAST_OPTION = TitleState::OPTIONS_SIZE - 1;

TitleState::TitleState()
:
	GameState( StateID::TITLE_STATE, { "Pale Purple", 2 } ),
	light_gradient_bg_ ( "bg/light_gradient.png", 400, 80, 0, Unit::WINDOW_HEIGHT_PIXELS - 100, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0 ),
	skyline_bg_ ( "bg/skyline.png", 264, 224, 0, 48, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0, -500 ),
	skyscrapers_bg_ ( "bg/title_skyscrapers.png", 248, 175, 0, Unit::WINDOW_HEIGHT_PIXELS - 175, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0, -1000 ),
	cloud_bg_ ( "bg/city_clouds.png", 400, 112, 0, 0, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, 0, -250, 0, 1, false, 128 ),
	logo_gfx_ ( "bosko_logo.png" ),
	window_box_ ( 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS ),
	logo_rect_ ( ( Unit::WINDOW_WIDTH_PIXELS - LOGO_WIDTH ) / 2, 16, LOGO_WIDTH, LOGO_HEIGHT ),
	created_by_ ( "Created by J.J.W. Mezun - 2017", 0, CREATED_BY_Y, Text::FontColor::BLACK, Text::FontAlign::CENTER ),
	option_bg_
	({{
		{ OPTIONS_X, OPTIONS_TOP_Y, OPTION_WIDTH_PIXELS, OPTION_HEIGHT_PIXELS },
		{ OPTIONS_X, OPTIONS_TOP_Y + ( ( OPTION_HEIGHT_PIXELS + OPTION_PADDING ) * 1 ), OPTION_WIDTH_PIXELS, OPTION_HEIGHT_PIXELS },
		{ OPTIONS_X, OPTIONS_TOP_Y + ( ( OPTION_HEIGHT_PIXELS + OPTION_PADDING ) * 2 ), OPTION_WIDTH_PIXELS, OPTION_HEIGHT_PIXELS },
		{ OPTIONS_X, OPTIONS_TOP_Y + ( ( OPTION_HEIGHT_PIXELS + OPTION_PADDING ) * 3 ), OPTION_WIDTH_PIXELS, OPTION_HEIGHT_PIXELS }
	}}),
	option_bg_shadows_
	({{
		{ option_bg_[ 0 ].x + SHADOW_LENGTH, option_bg_[ 0 ].y + SHADOW_LENGTH, OPTION_WIDTH_PIXELS, OPTION_HEIGHT_PIXELS },
		{ option_bg_[ 1 ].x + SHADOW_LENGTH, option_bg_[ 1 ].y + SHADOW_LENGTH, OPTION_WIDTH_PIXELS, OPTION_HEIGHT_PIXELS },
		{ option_bg_[ 2 ].x + SHADOW_LENGTH, option_bg_[ 2 ].y + SHADOW_LENGTH, OPTION_WIDTH_PIXELS, OPTION_HEIGHT_PIXELS },
		{ option_bg_[ 3 ].x + SHADOW_LENGTH, option_bg_[ 3 ].y + SHADOW_LENGTH, OPTION_WIDTH_PIXELS, OPTION_HEIGHT_PIXELS }
	}}),
	option_text_
	({{
		"New Game",
		"Load Game",
		"Options",
		"Quit"
	}}),
	selection_ ( 0 ),
	prev_selection_ ( 0 ),
	selection_timer_ ( 0 ),
	can_load_ ( false )
{
	Audio::changeSong( "title" );
};

TitleState::~TitleState() {};

void TitleState::stateUpdate()
{
	if ( Input::pressed( Input::Action::MOVE_UP ) )
	{
		subtractFromSelection();
		while ( invalidOption( selection_ ) )
		{
			subtractFromSelection();
		}
		selection_timer_ = 0;
	}
	else if ( Input::pressed( Input::Action::MOVE_DOWN ) )
	{
		addToSelection();
		while ( invalidOption( selection_ ) )
		{
			addToSelection();
		}
		selection_timer_ = 0;
	}
	else
	{
		++selection_timer_;
	}

	if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		switch( (Option)selection_ )
		{
			case ( Option::NEW ):
				Main::changeState( std::unique_ptr<GameState> ( new OverworldState() ) );
			break;

			case ( Option::LOAD ):
				Inventory::load();
				Main::changeState( std::unique_ptr<GameState> ( new OverworldState() ) );
			break;

			case ( Option::QUIT ):
				Main::quit();
			break;
		}
	}
	
	cloud_bg_.move( Unit::WINDOW_WIDTH_PIXELS, window_box_ );
	skyline_bg_.move( Unit::WINDOW_WIDTH_PIXELS, window_box_ );
	skyscrapers_bg_.move( Unit::WINDOW_WIDTH_PIXELS, window_box_ );
};

void TitleState::stateRender()
{
	Render::colorCanvas( 2 );
	logo_gfx_.render( logo_rect_, nullptr );
	light_gradient_bg_.render( window_box_ );
	skyline_bg_.render( window_box_ );
	skyscrapers_bg_.render( window_box_ );
	cloud_bg_.render( window_box_ );

	for ( int i = 0; i < option_text_.size(); ++i )
	{
		int bg_color = 5;
		Text::FontColor shade = Text::FontColor::WHITE;

		if ( i == selection_ )
		{
			switch ( selection_timer_ )
			{
				case ( 0 ):
				case ( 1 ):
					bg_color = 4;
					shade = Text::FontColor::LIGHT_GRAY;					
				break;
				case ( 2 ):
				case ( 3 ):
					bg_color = 3;
					shade = Text::FontColor::LIGHT_MID_GRAY;
				break;
				case ( 4 ):
				case ( 5 ):
					bg_color = 2;
					shade = Text::FontColor::DARK_MID_GRAY;
				break;
				default:
					bg_color = 1;
					shade = Text::FontColor::DARK_GRAY;
				break;
			}
		}
		else if ( i == prev_selection_ )
		{
			switch ( selection_timer_ )
			{
				case ( 0 ):
				case ( 1 ):
					bg_color = 2;
					shade = Text::FontColor::DARK_MID_GRAY;
				break;
				case ( 2 ):
				case ( 3 ):
					bg_color = 3;
					shade = Text::FontColor::LIGHT_MID_GRAY;				
				break;
				case ( 4 ):
				case ( 5 ):
					bg_color = 4;
					shade = Text::FontColor::LIGHT_GRAY;
				break;
				default:
					bg_color = 5;
					shade = Text::FontColor::WHITE;
				break;
			}
		}

		Render::renderRect( option_bg_shadows_[ i ], SHADOW_COLOR );
		int text_y = option_bg_[ i ].y;
		if ( invalidOption( i ) )
		{
			shade = Text::FontColor::LIGHT_MID_GRAY;
			text_y += 2;
		}
		else
		{
			Render::renderRect( option_bg_[ i ], bg_color );
		}

		Text::renderText( option_text_[ i ], 0, text_y + Unit::PIXELS_PER_MINIBLOCK, nullptr, shade, NULL, Text::FontAlign::CENTER );
	}

	created_by_.render();
};

void TitleState::init()
{
	std::ifstream ifs( Main::savePath() );

	if ( ifs.is_open() )
	{
		can_load_ = true;
	}
	else
	{
		can_load_ = false;
	}

	ifs.close();
	
	Inventory::reset();
};

void TitleState::subtractFromSelection()
{
	prev_selection_ = selection_;
	--selection_;
	if ( selection_ < 0 )
	{
		selection_ = LAST_OPTION;
	}
};

void TitleState::addToSelection()
{
	prev_selection_ = selection_;
	++selection_;
	if ( selection_ > LAST_OPTION )
	{
		selection_ = 0;
	}
};