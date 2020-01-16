#include "audio.hpp"
#include "config.hpp"
#include "controls_option_state.hpp"
#include "input.hpp"
#include "language_option_state.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "options_state.hpp"
#include "rapidjson/document.h"
#include "render.hpp"
#include "screen_option_state.hpp"
#include "wtext_obj.hpp"

LanguageOptionState::LanguageOptionState()
:
	GameState( StateID::OPTIONS_STATE, { "Mountain Red", 2 }, false ),
	bg_ (),
	title_ (),
	options_ ( OptionSystem::generateGridOptionSystem( Localization::getLanguageNames() ) )
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
	initTitle();
	options_.initForLanguageSelect();
	options_.setPressedDown( Localization::getCurrentLanguageIndex() );
};

void LanguageOptionState::updateInput()
{
	if ( Input::pressed( Input::Action::CANCEL ) )
	{
		Audio::playSound( Audio::SoundType::CANCEL );
		Main::changeState( std::make_unique<OptionsState> ( 2 ) );
	}
	else if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		Localization::setLanguage( options_.selection() );
		options_.setSelectedPressedDown();
		resetTitle();
		Audio::playSound( Audio::SoundType::CONFIRM );
		saveLanguageSettings();
	}
};

void LanguageOptionState::initTitle()
{
	WTextObj::generateTexture( title_, Localization::getCurrentLanguage().getLanguageOptionsTitle(), 0, 16, WTextCharacter::Color::WHITE, WTextObj::DEFAULT_WIDTH, WTextObj::Align::CENTER, WTextCharacter::Color::BLACK );
};

void LanguageOptionState::resetTitle()
{
	title_.destroy();
	initTitle();
}

void LanguageOptionState::saveLanguageSettings() const
{
	rapidjson::Document document = Config::readData();
	const std::string& language_value = Localization::getCurrentLanguage().getPathName();
	if ( document.HasMember( "language" ) )
	{
		document[ "language" ].SetString( language_value.data(), language_value.size(), document.GetAllocator() );
	}
	else
	{
		rapidjson::Value language;
		language.SetString( language_value.data(), language_value.size(), document.GetAllocator() );
		document.AddMember( "language", language, document.GetAllocator() );
	}
	Config::saveData( document );
}