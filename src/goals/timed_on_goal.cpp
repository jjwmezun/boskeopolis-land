#include "timed_on_goal.hpp"
#include "event_system.hpp"
#include "input.hpp"
#include "level_state.hpp"

TimedOnGoal::TimedOnGoal( std::u32string message, int time_limit_seconds )
:
	Goal( message ),
	time_limit_frames_ ( time_limit_seconds * Unit::FPS ),
    current_time_ ( 0 )
{};

TimedOnGoal::~TimedOnGoal() {};

void TimedOnGoal::update( LevelState& level_state )
{
    if ( level_state.events().isSwitchOn() )
    {
        if ( current_time_ == time_limit_frames_ )
        {
            level_state.events().forceSwitchOff();
            current_time_ = 0;
        }
        else
        {
            ++current_time_;
        }
    }
    else
    {
        current_time_ = 0;
    }
};