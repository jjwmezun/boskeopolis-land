#include "main.h"
#include "goal.h"

Goal::Goal( std::string message ) : message_ ( message ) {};
Goal::~Goal() {};

void Goal::update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events, const Input& input )
{
};

void Goal::init( Sprite& hero, InventoryLevel& inventory, EventSystem& events )
{
	Main::tempMessage( message(), { "Sunny Yellow", 2 }, Text::FontShade::DARK_GRAY );

	customInit( hero, inventory, events );
};

void Goal::customInit( Sprite& hero, InventoryLevel& inventory, EventSystem& events )
{
};

std::string Goal::message()
{
	return message_;
};