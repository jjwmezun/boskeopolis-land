#include "clock.hpp"
#include "survive_time_goal.hpp"
#include "event_system.hpp"
#include "inventory_level.hpp"
#include "level_state.hpp"
#include "mezun_helpers.hpp"
#include "text.hpp"

SurviveTimeGoal::SurviveTimeGoal( std::u32string message, int wait_time )
:
	Goal( formatMessage( message, wait_time ) ),
	wait_time_ ( wait_time )
{};

SurviveTimeGoal::~SurviveTimeGoal() {};

void SurviveTimeGoal::update( LevelState& level_state )
{
	if ( level_state.inventory().clock().totalSeconds() >= wait_time_ )
	{
		level_state.events().win();
	}
};

void SurviveTimeGoal::customInit( LevelState& level_state )
{
	level_state.inventory().clock().reset( Direction::Vertical::DOWN, wait_time_ );
};