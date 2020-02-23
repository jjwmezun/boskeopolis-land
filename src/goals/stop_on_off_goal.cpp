#include "audio.hpp"
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

StopOnOffGoal::StopOnOffGoal( std::u32string message ) : Goal( message ) {};
StopOnOffGoal::~StopOnOffGoal() {};

void StopOnOffGoal::update( LevelState& level_state )
{
	const int round_time = level_state.frame() % TOTAL_ROUND_TIME;
	switch ( round_time )
	{
		case ( 0 ):
		{
			level_state.events().forceSwitchOn();
			Audio::resumeSong();
			level_state.newPalette( "Go Green" );
		}
		break;

		case ( GO_TIME ):
		{
			level_state.newPalette( "Stop Red" );
		}
		break;

		case ( GO_TIME + BLINK_TIME ):
		{
			level_state.newPalette( "Go Green" );
		}
		break;

		case ( GO_TIME + ( BLINK_TIME * 2 ) ):
		{
			level_state.newPalette( "Stop Red" );
		}
		break;

		case ( GO_TIME + ( BLINK_TIME * 3 ) ):
		{
			level_state.newPalette( "Go Green" );
		}
		break;

		case ( GO_TIME + ( BLINK_TIME * 4 ) ):
		{
			level_state.newPalette( "Stop Red" );
		}
		break;

		case ( GO_TIME + ( BLINK_TIME * 5 ) ):
		{
			level_state.newPalette( "Go Green" );
		}
		break;

		case ( GO_TIME + ( BLINK_TIME * 6 ) ):
		{
			Audio::pauseSong();
			level_state.newPalette( "Stop Red" );
		}
		break;
	}

	if ( round_time > GO_TIME + ( BLINK_TIME * 6 ) + 8 )
	{
		level_state.events().forceSwitchOff();
	}

	switch ( level_state.events().isSwitchOn() )
	{
		case ( false ):
		{
			if
			(
				abs( level_state.sprites().hero().vx_ ) > 500 ||
				abs( level_state.sprites().hero().acceleration_x_ > 0 ) ||
				abs( level_state.sprites().hero().acceleration_y_ > 0 )
			)
			{
				level_state.events().fail();
			}
		}
		break;
	}
};

void StopOnOffGoal::customInit( LevelState& level_state )
{
	level_state.events().forceSwitchOn();
};
