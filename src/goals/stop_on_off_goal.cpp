#include "event_system.hpp"
#include "level_state.hpp"
#include "sprite.hpp"
#include "sprite_system.hpp"
#include "stop_on_off_goal.hpp"
#include "unit.hpp"

static constexpr int GO_TIME = 60;
static constexpr int BLINK_TIME = 4;
static constexpr int NUM_O_BLINKS = 5;
static constexpr int NUM_O_BLINKS_BOTH_COLORS = NUM_O_BLINKS * 2;
static constexpr int STOP_TIME = 30;
static constexpr int TOTAL_ROUND_TIME = GO_TIME + ( NUM_O_BLINKS_BOTH_COLORS * BLINK_TIME ) + STOP_TIME;

StopOnOffGoal::StopOnOffGoal() : Goal( "¡Don't move when the light's red!" ) {};
StopOnOffGoal::~StopOnOffGoal() {};

void StopOnOffGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state )
{
	const int round_time = state.frame() % TOTAL_ROUND_TIME;

	switch ( round_time )
	{
		case ( 0 ):
			events.switch_ = true;
			state.newPalette( "Go Green" );
		break;

		case ( GO_TIME ):
			state.newPalette( "Stop Red" );
		break;

		case ( GO_TIME + BLINK_TIME ):
			state.newPalette( "Go Green" );
		break;

		case ( GO_TIME + ( BLINK_TIME * 2 ) ):
			state.newPalette( "Stop Red" );
		break;

		case ( GO_TIME + ( BLINK_TIME * 3 ) ):
			state.newPalette( "Go Green" );
		break;

		case ( GO_TIME + ( BLINK_TIME * 4 ) ):
			state.newPalette( "Stop Red" );
		break;

		case ( GO_TIME + ( BLINK_TIME * 5 ) ):
			state.newPalette( "Go Green" );
		break;

		case ( GO_TIME + ( BLINK_TIME * 6 ) ):
			events.switch_ = false;
			state.newPalette( "Stop Red" );
		break;
	}

	switch ( events.switch_ )
	{
		case ( false ):
		{
			if
			(
				abs( sprites.hero().vx_ ) > 500 ||
				abs( sprites.hero().acceleration_x_ > 0 ) ||
				abs( sprites.hero().acceleration_y_ > 0 )
			)
			{
				events.fail();
			}
		}
		break;
	}
};

void StopOnOffGoal::customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
	events.switch_ = true;
};
