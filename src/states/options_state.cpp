#include "audio.hpp"
#include "controls_option_state.hpp"
#include "input.hpp"
#include "main.hpp"
#include "options_state.hpp"
#include "render.hpp"
#include "screen_option_state.hpp"
#include "text_info.hpp"

static constexpr int NUMBER_OF_OPTIONS = 3;

OptionsState::OptionsState()
:
	GameState( StateID::OPTIONS_STATE, { "Mountain Red", 2 }, false ),
	bg_ (),
	title_ ( WTextObj::generateTexture( TextInfo::getOptionsTitle(), 0, 16, WTextObj::Color::WHITE, WTextObj::DEFAULT_WIDTH, WTextObj::Align::CENTER, WTextObj::Color::BLACK ) ),
	options_ ( TextInfo::getOptionsOptions(), 20, 64 )
{
	Audio::changeSong( "level-select" );
};

OptionsState::~OptionsState()
{
	title_.destroy();
};

void OptionsState::stateUpdate()
{
	bg_.update();
	options_.update();
	updateInput();
};

void OptionsState::stateRender()
{
	bg_.render();
	options_.render();
	title_.render();
};

void OptionsState::init()
{
	options_.init();
};

void OptionsState::updateInput()
{
	if ( Input::pressed( Input::Action::CANCEL ) )
	{
		Audio::playSound( Audio::SoundType::CANCEL );
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
		}
		Audio::playSound( Audio::SoundType::CONFIRM );
	}
};
