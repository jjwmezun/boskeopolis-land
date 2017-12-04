#include "main.hpp"
#include "message_state.hpp"
#include "goal.hpp"

Goal::Goal( std::string message ) : message_ ( message ) {};
Goal::~Goal() {};

void Goal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state )
{
};

void Goal::init( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
	Main::pushState
	(
		std::make_unique<MessageState> ( message(), MessageState::Type::POP, Palette( "Caution Yellow", 3 ), nullptr, Text::FontColor::WHITE, Text::FontColor::DARK_GRAY, "jingle", false, false )
	);
	customInit( hero, level, inventory_screen, events, health );
};

void Goal::customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
};

std::string Goal::message()
{
	return message_;
};