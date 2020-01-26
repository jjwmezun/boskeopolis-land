#include "audio.hpp"
#include "controls_option_state.hpp"
#include "input.hpp"
#include "language_option_state.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "options_state.hpp"
#include "render.hpp"
#include "screen_option_state.hpp"
#include "title_state.hpp"
#include "wtext_obj.hpp"

static constexpr int NUMBER_OF_OPTIONS = 3;

OptionsState::OptionsState( bool from_title )
:
	GameState( StateID::OPTIONS_STATE, { "Mountain Red", 2 }, false ),
	from_title_ ( from_title ),
	bg_ (),
	title_ (),
	options_ ( OptionSystem::generateVerticalOptionSystem( Localization::getCurrentLanguage().getOptionsOptions(), 64 ) )
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
	WTextObj::generateTexture( title_, Localization::getCurrentLanguage().getOptionsTitle(), 0, 16, WTextCharacter::Color::WHITE, WTextObj::DEFAULT_WIDTH, WTextObj::Align::CENTER, WTextCharacter::Color::BLACK );
	options_.init();
};

void OptionsState::updateInput()
{
	if ( Input::pressed( Input::Action::CANCEL ) )
	{
		Audio::playSound( Audio::SoundType::CANCEL );
		if ( from_title_ )
		{
			Main::changeState( std::make_unique<TitleState> () );
		}
		else
		{
			Main::popState( true );
		}
	}
	else if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		switch ( ( Option )( options_.selection() ) )
		{
			case ( Option::RESOLUTION ):
			{
				Main::pushState( std::make_unique<ScreenOptionState> (), true );
			}
			break;
			case ( Option::CONTROLS ):
			{
				Main::pushState( std::make_unique<ControlsOptionState> (), true );
			}
			break;
			case ( Option::LANGUAGE ):
			{
				Main::pushState( std::make_unique<LanguageOptionState> (), true );
			}
			break;
		}
		options_.setSelectedVisuallyPressedDown();
		Audio::playSound( Audio::SoundType::CONFIRM );
	}
};

void OptionsState::backFromPop()
{
	options_.setAllNotPressedDown();
};