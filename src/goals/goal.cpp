#include "main.hpp"
#include "goal.hpp"

Goal::Goal( std::string message ) : message_ ( message ) {};
Goal::~Goal() {};

void Goal::update( SpriteSystem& sprites, const Map& lvmap, EventSystem& events, Health& health )
{
};

void Goal::init( Sprite& hero, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
	Main::tempMessage( message(), { "Sunny Yellow", 2 }, Text::FontShade::DARK_GRAY );
	customInit( hero, inventory_screen, events, health );
};

void Goal::customInit( Sprite& hero, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
};

std::string Goal::message()
{
	return message_;
};