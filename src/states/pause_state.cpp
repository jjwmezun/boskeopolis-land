#include "main.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "pause_state.hpp"
#include "level_select_state.hpp"

constexpr sdl::rect PauseState::surface_box_;

PauseState::PauseState( const Palette& palette, EventSystem& events )
:
	GameState( StateID::PAUSE_STATE, palette ),
	option_text_ ( optionText( Inventory::victory() ) ),
	option_selection_ ( PauseOption::PO_CONTINUE ),
	events_ ( events )
{};

std::vector<Text> PauseState::optionText( bool beaten ) const
{
	std::vector<Text> temp;

	temp.push_back( Text( "Continue", surface_box_.x + 8, surface_box_.y + 16, Text::FontShade::LIGHT_MID_GRAY ) );

	if ( beaten )
	{
		temp.push_back( Text( "Back to Level Select", surface_box_.x + 8, surface_box_.y + 32, Text::FontShade::LIGHT_MID_GRAY ) );
	}
	else
	{
		temp.push_back( Text( "Give Up", surface_box_.x + 8, surface_box_.y + 32, Text::FontShade::LIGHT_MID_GRAY ) );
	}

	return temp;
};

PauseState::~PauseState() {};

void PauseState::update()
{
	if ( Input::pressed( Input::Action::MOVE_UP ) || Input::pressed( Input::Action::MOVE_DOWN ) )
	{
		switch ( option_selection_ )
		{
			case ( PauseOption::PO_CONTINUE ):
				option_selection_ = PauseOption::PO_QUIT;
			break;
			case ( PauseOption::PO_QUIT ):
				option_selection_ = PauseOption::PO_CONTINUE;
			break;
		}
	}

	if ( Input::pressed( Input::Action::CONFIRM ) )
	{

		if ( option_selection_ == PauseOption::PO_QUIT )
		{
			events_.quitLevel();
		}

		Main::popState();
	}

	if ( Input::pressed( Input::Action::MENU ) )
	{
		Main::popState();
	}
};

void PauseState::stateRender()
{
	Render::renderRect( surface_box_, 6 );

	for ( int i = 0; i < NUM_O_PAUSE_OPTIONS; ++i )
	{
		Text::FontShade text_color = Text::FontShade::__NULL;

		if ( palette().neon() )
		{
			text_color = Text::FontShade::BLACK;
		}
		else if ( ( int )option_selection_ == i )
		{
			text_color = Text::FontShade::WHITE;
		}

		option_text_.at( i ).render( nullptr, text_color );
	}
};

void PauseState::init()
{
};