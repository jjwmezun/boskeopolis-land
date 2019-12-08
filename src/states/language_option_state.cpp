#include "audio.hpp"
#include "controls_option_state.hpp"
#include "input.hpp"
#include "language_option_state.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "render.hpp"
#include "screen_option_state.hpp"

static constexpr int NUMBER_OF_OPTIONS = 3;

LanguageOptionState::LanguageOptionState()
:
	GameState( StateID::OPTIONS_STATE, { "Mountain Red", 2 }, false ),
	bg_ (),
	title_ ( WTextObj::generateTexture( Localization::getCurrentLanguage().getLanguageOptionsTitle(), 0, 16, WTextObj::Color::WHITE, WTextObj::DEFAULT_WIDTH, WTextObj::Align::CENTER, WTextObj::Color::BLACK ) ),
	options_ ( Localization::getLanguageNames(), 20, 64 )
{
	Audio::changeSong( "level-select" );
};

LanguageOptionState::~LanguageOptionState()
{
	title_.destroy();
};

void LanguageOptionState::stateUpdate()
{
	bg_.update();
	options_.update();
	updateInput();
};

void LanguageOptionState::stateRender()
{
	bg_.render();
	options_.render();
	title_.render();
};

void LanguageOptionState::init()
{
	options_.init();
};

void LanguageOptionState::updateInput()
{
	if ( Input::pressed( Input::Action::CANCEL ) )
	{
		Audio::playSound( Audio::SoundType::CANCEL );
		Main::popState();
	}
	else if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		Localization::setLanguage( options_.selection() );
		Audio::playSound( Audio::SoundType::CONFIRM );
	}
};
