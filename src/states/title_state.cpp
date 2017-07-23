#include "game.h"
#include "input.h"
#include <fstream>
#include "overworld_state.h"
#include "title_state.h"

constexpr sdl2::SDLRect TitleState::logo_rect_;

TitleState::TitleState() : GameState( StateID::TITLE_STATE ) {};

TitleState::~TitleState() {};

void TitleState::update( Game& game, const Input& input )
{
	if ( input.pressed( Input::Action::MOVE_UP ) )
	{
		--selection_;
	}
	else if ( input.pressed( Input::Action::MOVE_DOWN ) )
	{
		++selection_;
	}

	highlight_rect_.y = OPTIONS_TOP_Y + Unit::MiniBlocksToPixels( selection_.value() );

	if ( input.pressed( Input::Action::CONFIRM ) )
	{
		switch( (Option)selection_.value() )
		{
			case ( Option::NEW ):
				game.changeState( std::unique_ptr<GameState> ( new OverworldState( false ) ) );
			break;

			case ( Option::LOAD ):
				game.changeState( std::unique_ptr<GameState> ( new OverworldState( true ) ) );
			break;

			case ( Option::QUIT ):
				game.quit();
			break;
		}
	}
};

void TitleState::stateRender()
{
	Render::renderRect( highlight_rect_, 6 );

	for ( int i = 0; i < option_text_.size(); ++i )
	{
		Text::FontShade shade = Text::FontShade::BLACK;

		if ( i == (int)Option::OPTIONS || ( !can_load_ && i == (int)Option::LOAD ) )
		{
			shade = Text::FontShade::LIGHT_MID_GRAY;
		}
		else if ( i == selection_.value() )
		{
			shade = Text::FontShade::WHITE;
		}

		Text::renderText( option_text_.at( i ), 0, OPTIONS_TOP_Y + Unit::MiniBlocksToPixels( i ), nullptr, shade, NULL, Text::FontAlign::CENTER );
	}

	logo_gfx_.render( logo_rect_, nullptr );
};

void TitleState::init( Game& game )
{
	std::ifstream ifs( Game::savePath() );

	if ( ifs.is_open() )
	{
		can_load_ = true;
	}
	else
	{
		can_load_ = false;
	}

	ifs.close();
};