
// Name
//===================================
//
// TimerRepeat
//


// DEPENDENCIES
//===================================

    #include "timer_repeat.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    TimerRepeat::TimerRepeat( int limit, bool starts_on, int start_count, Direction::Vertical direction )
    :
        Timer( limit, starts_on, start_count, direction )
    {};

    TimerRepeat::~TimerRepeat() {};

    void TimerRepeat::start()
    {
        on_ = true;
        restartCounter();
        hit_ = false;
    };

    void TimerRepeat::stop()
    {
        on_ = false;
        restartCounter();
        hit_ = false;
    };

    void TimerRepeat::event( bool hit )
    {
        if ( hit )
        {
            hit_ = true;
            restartCounter();
        }
        else
        {
            hit_ = false;
        }
    };

    bool TimerRepeat::hit() const { return hit_; };
