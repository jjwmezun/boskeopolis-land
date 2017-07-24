#include "input.hpp"
#include "main.hpp"
#include "overworld_menu_state.hpp"
#include "title_state.hpp"

constexpr sdl2::SDLRect OverworldMenuState::bg_;

OverworldMenuState::OverworldMenuState( bool& go_to_list, bool& camera_mode, const Palette& pal )
:
	GameState( StateID::OVERWORLD_MENU_STATE, pal ),
	option_selection_ ( ( int )( Option::CONTINUE ) ),
	go_to_list_ ( go_to_list ),
	camera_mode_ ( camera_mode )
{};

OverworldMenuState::~OverworldMenuState() {};

void OverworldMenuState::update()
{
	if ( Input::pressed( Input::Action::MENU ) )
	{
		Main::popState();
	}

	if ( Input::pressed( Input::Action::MOVE_DOWN ) )
	{
		++option_selection_;
	}
	else if ( Input::pressed( Input::Action::MOVE_UP ) )
	{
		--option_selection_;
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
	}
};

void OverworldMenuState::stateRender()
{
	Render::renderRect( bg_, 6 );

	for ( int i = 0; i < NUM_O_OPTIONS; ++i )
	{
		Text::FontShade text_color = Text::FontShade::__NULL;

		if ( option_selection_ == i )
		{
			text_color = Text::FontShade::WHITE;
		}

		option_text_.at( i ).render( nullptr, text_color );
	}
};

void OverworldMenuState::init()
{
};