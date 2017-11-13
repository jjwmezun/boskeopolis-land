#include "main.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "pause_state.hpp"
#include "level_select_state.hpp"

static constexpr int PAUSE_BOX_WIDTH = 24;
static constexpr int PAUSE_BOX_HEIGHT = 7;

PauseState::PauseState( const Palette& palette, EventSystem& events )
:
	GameState( StateID::PAUSE_STATE, palette ),
	surface_box_
	(
		Unit::MiniBlocksToPixels( floor( Unit::WINDOW_WIDTH_MINIBLOCKS / 2 ) - floor( PAUSE_BOX_WIDTH / 2 ) ),
		Unit::MiniBlocksToPixels( floor( Unit::WINDOW_HEIGHT_MINIBLOCKS / 2 ) - floor( PAUSE_BOX_HEIGHT / 2 ) ),
		Unit::MiniBlocksToPixels( PAUSE_BOX_WIDTH ),
		Unit::MiniBlocksToPixels( PAUSE_BOX_HEIGHT )
	),
	option_text_
	({
		{ "Continue", surface_box_.x + 8, surface_box_.y + 16, Text::FontColor::LIGHT_MID_GRAY },
		{ quitName( Inventory::victory() ), surface_box_.x + 8, surface_box_.y + 32, Text::FontColor::LIGHT_MID_GRAY }
	}),
	events_ ( events ),
	option_selection_ ( PauseOption::PO_CONTINUE )
{};

std::string PauseState::quitName( bool beaten ) const
{
	return ( beaten ) ? "Back to Level Select" : "Give Up";
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
	else if ( Input::pressed( Input::Action::MENU ) )
	{
		Main::popState();
	}
};

void PauseState::stateRender()
{
	Render::renderRect( surface_box_, 6 );

	for ( int i = 0; i < NUM_O_PAUSE_OPTIONS; ++i )
	{
		Text::FontColor text_color = Text::FontColor::__NULL;

		if ( ( int )( option_selection_ ) == i )
		{
			text_color = Text::FontColor::WHITE;
		}

		option_text_[ i ].render( nullptr, text_color );
	}
};