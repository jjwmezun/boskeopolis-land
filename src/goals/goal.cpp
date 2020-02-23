#include "main.hpp"
#include "mezun_helpers.hpp"
#include "wmessage_state.hpp"
#include "goal.hpp"

Goal::Goal( std::u32string message ) : message_ ( message ) {};
Goal::~Goal() {};

void Goal::update( LevelState& level_state ) {};

void Goal::init( LevelState& level_state )
{
	Main::pushState
	(
		std::make_unique<WMessageState>
		(
			WTextObj::MessageData
			{
				message_,
				WTextCharacter::Color::WHITE,
				WTextCharacter::Color::DARK_GRAY
			},
			WMessageState::Type::POP,
			Palette( "Caution Yellow", 3 ),
			nullptr,
			"jingle",
			false,
			false
		)
	);
	customInit( level_state );
};

void Goal::customInit( LevelState& level_state ) {};

std::u32string Goal::formatMessage( std::u32string message, int amount_needed )
{
	return mezun::stringReplace( message, U"%n", mezun::intToChar32String( amount_needed ) );
};