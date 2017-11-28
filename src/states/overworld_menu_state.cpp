#include "audio.hpp"
#include "input.hpp"
#include "main.hpp"
#include "overworld_menu_state.hpp"
#include "title_state.hpp"

static constexpr int BG_WIDTH = 24;
static constexpr int BG_HEIGHT = 11;

OverworldMenuState::OverworldMenuState( bool& go_to_list, bool& camera_mode, const Palette& pal )
:
	GameState( StateID::OVERWORLD_MENU_STATE, pal ),	
	bg_
	(
		Unit::MiniBlocksToPixels( floor( Unit::WINDOW_WIDTH_MINIBLOCKS / 2 ) - floor( BG_WIDTH / 2 ) ),
		Unit::MiniBlocksToPixels( floor( Unit::WINDOW_HEIGHT_MINIBLOCKS / 2 ) - floor( BG_HEIGHT / 2 ) ),
		Unit::MiniBlocksToPixels( BG_WIDTH ),
		Unit::MiniBlocksToPixels( BG_HEIGHT )
	),
	option_text_
	({{
		TextObj( "Continue", bg_.x + 8, bg_.y + 16, Text::FontColor::LIGHT_MID_GRAY ),
		TextObj( "Level List", bg_.x + 8, bg_.y + 32, Text::FontColor::LIGHT_MID_GRAY ),
		TextObj( "Camera View", bg_.x + 8, bg_.y + 48, Text::FontColor::LIGHT_MID_GRAY ),
		TextObj( "Quit", bg_.x + 8, bg_.y + 64, Text::FontColor::LIGHT_MID_GRAY )
	}}),
	go_to_list_ ( go_to_list ),
	camera_mode_ ( camera_mode ),
	option_selection_ ( ( int )( Option::CONTINUE ) )
{
	Audio::playSound( Audio::SoundType::PAUSE );
};

OverworldMenuState::~OverworldMenuState() {};

void OverworldMenuState::stateUpdate()
{
	if ( Input::pressed( Input::Action::MENU ) )
	{
		Main::popState();
		Audio::playSound( Audio::SoundType::CANCEL );
	}

	if ( Input::pressed( Input::Action::MOVE_DOWN ) )
	{
		++option_selection_;
		Audio::playSound( Audio::SoundType::SELECT );
	}
	else if ( Input::pressed( Input::Action::MOVE_UP ) )
	{
		--option_selection_;
		Audio::playSound( Audio::SoundType::SELECT );
	}
	
	if ( option_selection_ >= NUM_O_OPTIONS )
	{
		option_selection_ = 0;
	}
	else if ( option_selection_ < 0 )
	{
		option_selection_ = NUM_O_OPTIONS - 1;
	}

	if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		switch ( option_selection_ )
		{
			case ( ( int )( Option::CONTINUE ) ):
				Main::popState();
			break;

			case ( ( int )( Option::LIST ) ):
				go_to_list_ = true;
				Main::popState();
			break;

			case ( ( int )( Option::CAMERA ) ):
				camera_mode_ = true;
				Main::popState();
			break;

			case ( ( int )( Option::QUIT ) ):
				Main::changeState( std::make_unique<TitleState> () );
			break;
		}
		Audio::playSound( Audio::SoundType::CONFIRM );
	}
};

void OverworldMenuState::stateRender()
{
	Render::renderRect( bg_, 6 );

	for ( int i = 0; i < NUM_O_OPTIONS; ++i )
	{
		option_text_[ i ].color_ = Text::FontColor::LIGHT_MID_GRAY;

		if ( option_selection_ == i )
		{
			option_text_[ i ].color_ = Text::FontColor::WHITE;
		}

		option_text_[ i ].render( nullptr );
	}
};