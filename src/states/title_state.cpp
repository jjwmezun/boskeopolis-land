#include "main.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include <fstream>
#include "overworld_state.hpp"
#include "title_state.hpp"

static constexpr int LOGO_WIDTH = 367;
static constexpr int LOGO_HEIGHT = 38;
static constexpr int LOGO_Y = 16;
static constexpr int CREATED_BY_Y = LOGO_HEIGHT + LOGO_Y + 8;
static constexpr int CREATED_BY_HEIGHT = 8;
static constexpr int OPTIONS_TOP_PADDING = 32;
static constexpr int OPTIONS_TOP_Y = CREATED_BY_Y + CREATED_BY_HEIGHT + OPTIONS_TOP_PADDING;

TitleState::TitleState()
:
	GameState( StateID::TITLE_STATE, { "Pale Purple", 2 } ),
	bg_ ( "bg/neon-city_2.png", 416, 224, 0, 0, 1, 1, 1, MapLayerImage::REPEAT_INFINITE, MapLayerImage::REPEAT_INFINITE, -500 ),
	logo_gfx_ ( "bosko_logo.png" ),
	logo_rect_ ( ( Unit::WINDOW_WIDTH_PIXELS - LOGO_WIDTH ) / 2, 16, LOGO_WIDTH, LOGO_HEIGHT ),
	created_by_ ( "Created by J.J.W. Mezun - 2017", 0, CREATED_BY_Y, Text::FontColor::BLACK, Text::FontAlign::CENTER ),
	highlight_rect_ ( 16, OPTIONS_TOP_Y, Unit::WINDOW_WIDTH_PIXELS - 32, 8 ),
	option_text_
	({{
		"New Game",
		"Load Game",
		"Options",
		"Quit"
	}}),
	selection_ ( 0, OPTIONS_SIZE - 1 ),
	can_load_ ( false )
{};

TitleState::~TitleState() {};

void TitleState::update()
{
	if ( Input::pressed( Input::Action::MOVE_UP ) )
	{
		--selection_;
	}
	else if ( Input::pressed( Input::Action::MOVE_DOWN ) )
	{
		++selection_;
	}

	highlight_rect_.y = OPTIONS_TOP_Y + Unit::MiniBlocksToPixels( selection_.value() );

	if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		switch( (Option)selection_.value() )
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
	
	bg_.move( Unit::WINDOW_WIDTH_PIXELS );
};

void TitleState::stateRender()
{
	bg_.render( { 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS } );
	Render::renderRect( highlight_rect_, 6 );

	for ( int i = 0; i < option_text_.size(); ++i )
	{
		Text::FontColor shade = Text::FontColor::BLACK;

		if ( i == (int)Option::OPTIONS || ( !can_load_ && i == (int)Option::LOAD ) )
		{
			shade = Text::FontColor::LIGHT_MID_GRAY;
		}
		else if ( i == selection_.value() )
		{
			shade = Text::FontColor::WHITE;
		}

		Text::renderText( option_text_.at( i ), 0, OPTIONS_TOP_Y + Unit::MiniBlocksToPixels( i ), nullptr, shade, NULL, Text::FontAlign::CENTER );
	}

	logo_gfx_.render( logo_rect_, nullptr );
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