
// Name
//===================================
//
// TimerSimple
//


// DEPENDENCIES
//===================================

    #include "timer_simple.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    TimerSimple::TimerSimple( int limit, bool starts_on, int start_count, Direction::Vertical direction )
    :
        Timer( limit, starts_on, start_count, direction ),
		done_ ( false )
    {};

    TimerSimple::~TimerSimple() {};

    void TimerSimple::start()
    {
        on_ = true;
        restartCounter();
        done_ = false;
    };

    void TimerSimple::event( bool hit )
    {
        if ( hit )
        {
            done_ = true;
            on_ = false;
        }
    };

    bool TimerSimple::done() const { return done_; };

    bool TimerSimple::hit() const { return done(); };

    void TimerSimple::stop()
    {
        on_ = false;
        restartCounter();
        done_ = false;
    };
