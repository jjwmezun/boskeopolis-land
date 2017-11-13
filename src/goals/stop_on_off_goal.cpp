#include "event_system.hpp"
#include "stop_on_off_goal.hpp"

StopOnOffGoal::StopOnOffGoal() : Goal( "¡Don't move when the light's red!" ) {};
StopOnOffGoal::~StopOnOffGoal() {};

void StopOnOffGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
};

void StopOnOffGoal::customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
	events.switch_ = true;
};