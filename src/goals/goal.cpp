#include "main.hpp"
#include "mezun_helpers.hpp"
#include "wmessage_state.hpp"
#include "goal.hpp"

Goal::Goal( std::u32string message ) : message_ ( message ) {};
Goal::~Goal() {};

void Goal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) {};

void Goal::init( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
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
	customInit( hero, level, inventory_screen, events, health );
};

void Goal::customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health ) {};

std::u32string Goal::formatMessage( std::u32string message, int amount_needed )
{
	return mezun::stringReplace( message, U"%n", mezun::intToChar32String( amount_needed ) );
};