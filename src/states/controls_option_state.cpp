#include "audio.hpp"
#include "controls_option_state.hpp"
#include "controls_option_prompt_state.hpp"
#include "input.hpp"
#include "main.hpp"
#include "render.hpp"
#include "screen_option_state.hpp"

static constexpr int OPTION_NAME_X = 16;
static constexpr int OPTION_NAME_START_Y = 48;

static int calculateKeyNameX()
{
	return OPTION_NAME_X + ( Input::calculateMaxActionNameLength() * Text::CHAR_SIZE_PIXELS ) + ( Text::CHAR_SIZE_PIXELS * 2 );
};

ControlsOptionState::ControlsOptionState()
:
	GameState( StateID::OPTIONS_STATE, { "Mountain Red", 2 }, false ),
	title_
	(
		"Controls",
		0,
		16,
		Text::FontColor::WHITE,
		Text::FontAlign::CENTER,
		Text::FontColor::BLACK
	),
	option_names_ (),
	key_names_ (),
	button_names_ (),
	selection_ ( Input::NUM_O_ACTIONS - 1 )
{
	const int KEY_NAME_X = calculateKeyNameX();
	int y = OPTION_NAME_START_Y;

	for ( int i = 0; i < Input::NUM_O_ACTIONS; i++ )
	{
		option_names_.emplace_back
		(
			Input::getActionName( ( Input::Action )( i ) ),
			OPTION_NAME_X,
			y,
			Text::FontColor::WHITE,
			Text::FontAlign::LEFT,
			Text::FontColor::BLACK
		);

		key_names_.emplace_back
		(
			Input::getKeyName( ( Input::Action )( i ) ),
			KEY_NAME_X,
			y,
			Text::FontColor::WHITE,
			Text::FontAlign::LEFT,
			Text::FontColor::BLACK
		);

		y += Text::CHAR_SIZE_PIXELS;
	}
};

ControlsOptionState::~ControlsOptionState() {};

void ControlsOptionState::stateUpdate()
{
	updateSelection();
	updateInput();
};

void ControlsOptionState::stateRender()
{
	Render::colorCanvas( 2 );
	renderOptions();
	title_.render();
};

void ControlsOptionState::init() {};

void ControlsOptionState::updateSelection()
{
	selection_.update();
	for ( int i = 0; i < Input::NUM_O_ACTIONS; i++ )
	{
		if ( i == selection_.selection() )
		{
			key_names_[ i ].color_ = Text::FontColor::LIGHT_GRAY;
		}
		else
		{
			key_names_[ i ].color_ = Text::FontColor::WHITE;
		}
	}
}

void ControlsOptionState::updateInput()
{
	if ( Input::pressed( Input::Action::CANCEL ) )
	{
		Main::popState();
	}
	else if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		Main::pushState( std::make_unique<ControlsOptionPromptState> () );
	}
};

void ControlsOptionState::renderOptions() const
{
	for ( int i = 0; i < Input::NUM_O_ACTIONS; ++i )
	{
		option_names_[ i ].render();
		key_names_[ i ].render();
	}
};
