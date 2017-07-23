
// Name
//===================================
//
// TimerRepeat
//


// DEPENDENCIES
//===================================

    #include "timer_repeat.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    TimerRepeat::TimerRepeat( int limit, bool starts_on, int start_count, Direction::Vertical direction )
    :
        Timer( limit, starts_on, start_count, direction ),
		hit_ ( false ),
		num_o_hits_ ( 0 )
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
			++num_o_hits_;
            restartCounter();
        }
        else
        {
            hit_ = false;
        }
    };

    bool TimerRepeat::hit() const { return hit_; };

	int TimerRepeat::numOHits() const { return num_o_hits_; };
