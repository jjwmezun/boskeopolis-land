#include "audio.hpp"
#include "controls_option_state.hpp"
#include "input.hpp"
#include "main.hpp"
#include "options_state.hpp"
#include "render.hpp"
#include "screen_option_state.hpp"

static constexpr int NUMBER_OF_OPTIONS = 3;

OptionsState::OptionsState()
:
	GameState( StateID::OPTIONS_STATE, { "Mountain Red", 2 }, false ),
	title_ ( "Options", 0, 16, Text::FontColor::WHITE, Text::FontAlign::CENTER, Text::FontColor::BLACK ),
	options_ ( { "Screen Resolution", "Controls", "Sound" }, 20, 64 )
{};

OptionsState::~OptionsState() {};

void OptionsState::stateUpdate()
{
	options_.update();
	updateInput();
};

void OptionsState::stateRender()
{
	Render::colorCanvas( 2 );
	options_.render();
	title_.render();
};

void OptionsState::init() {};

void OptionsState::updateInput()
{
	if ( Input::pressed( Input::Action::CANCEL ) )
	{
		Main::popState();
	}
	else if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		switch ( ( Option )( options_.selection() ) )
		{
			case ( Option::RESOLUTION ):
				Main::pushState( std::make_unique<ScreenOptionState> () );
			break;
			case ( Option::CONTROLS ):
				Main::pushState( std::make_unique<ControlsOptionState> () );
			break;
			case ( Option::SOUND ):
				Main::popState();
			break;
		}
		Audio::playSound( Audio::SoundType::CONFIRM );
	}
};
