#include "main.hpp"
#include "goal.hpp"

Goal::Goal( std::string message ) : message_ ( message ) {};
Goal::~Goal() {};

void Goal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
};

void Goal::init( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
	Main::tempMessage( message(), { "Sunny Yellow", 2 }, Text::FontShade::DARK_GRAY );
	customInit( hero, level, inventory_screen, events, health );
};

void Goal::customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
};

std::string Goal::message()
{
	return message_;
};