
// Name
//===================================
//
// TimerSwitch
//


// DEPENDENCIES
//===================================

    #include "timer_switch.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    TimerSwitch::TimerSwitch( int limit, bool starts_on, int start_count, Direction::Vertical direction )
    :
        Timer( limit, starts_on, start_count, direction )
    {};

    TimerSwitch::~TimerSwitch() {};

    void TimerSwitch::start()
    {
        on_ = true;
        restartCounter();
        switch_ = false;
    };

    void TimerSwitch::stop()
    {
        on_ = false;
        restartCounter();
        switch_ = false;
    };

    void TimerSwitch::event( bool hit )
    {
        if ( hit )
        {
            changeSwitch();
            restartCounter();
        }
    };

    bool TimerSwitch::switchStatus() const { return switch_; };

    void TimerSwitch::changeSwitch()
    {
        switch_ = !switch_;
    };

