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
	bg_ (),
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
	selection_ ( Input::NUM_O_ACTIONS - 1 ),
	reset_option_names_ ( false )
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

		button_names_.emplace_back
		(
			Input::getButtonName( ( Input::Action )( i ) ),
			Unit::WINDOW_WIDTH_PIXELS - 72,
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
	bg_.update();
	testResetOptionNames();
	updateSelection();
	updateInput();
};

void ControlsOptionState::stateRender()
{
	bg_.render();
	renderOptions();
	title_.render();
};

void ControlsOptionState::init() {};

void ControlsOptionState::updateSelection()
{
	selection_.update();
	for ( int i = 0; i < Input::NUM_O_ACTIONS; i++ )
	{
		const Text::FontColor color = ( i == selection_.selection() ) ? Text::FontColor::LIGHT_GRAY : Text::FontColor::WHITE;
		key_names_[ i ].color_ = option_names_[ i ].color_ = button_names_[ i ].color_ = color;
	}
}

void ControlsOptionState::updateInput()
{
	if ( Input::pressed( Input::Action::CANCEL ) )
	{
		Audio::playSound( Audio::SoundType::CANCEL );
		Main::popState();
	}
	else if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		reset_option_names_ = true;
		Audio::playSound( Audio::SoundType::CONFIRM );
		Main::pushState( std::make_unique<ControlsOptionPromptState> ( ( Input::Action )( selection_.selection() ) ) );
	}
};

void ControlsOptionState::renderOptions() const
{
	for ( int i = 0; i < Input::NUM_O_ACTIONS; ++i )
	{
		option_names_[ i ].render();
		key_names_[ i ].render();
		button_names_[ i ].render();
	}
};

void ControlsOptionState::resetOptionNames()
{
	for ( int i = 0; i < Input::NUM_O_ACTIONS; i++ )
	{
		key_names_[ i ].words_ = Input::getKeyName( ( Input::Action )( i ) );
		button_names_[ i ].words_ = Input::getButtonName( ( Input::Action )( i ) );
	}
};

void ControlsOptionState::testResetOptionNames()
{
	if ( reset_option_names_ )
	{
		resetOptionNames();
		reset_option_names_ = false;
	}
};
