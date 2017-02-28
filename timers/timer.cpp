
// Name
//===================================
//
// Timer
//


// DEPENDENCIES
//===================================

    #include "timer.h"
    #include <iostream>


// STATIC PROPERTIES
//===================================

// METHODS
//===================================

    Timer::Timer( int limit, bool starts_on, int start_count, Direction::Vertical direction )
    :
        limit_ ( ( limit != NULL ) ? limit : NORMAL_FRAMES ), // Treat NULL as default.
        on_ ( starts_on ),
        starts_on_ ( starts_on ),
        counter_ ( start_count ),
        direction_ ( direction )
    {};

    Timer::~Timer() {};

    void Timer::pause() { on_ = false; };

    void Timer::resume() { on_ = true; };

	void Timer::restart() { start(); };

    void Timer::start()
    {
        on_ = true;
        restartCounter();
    };

    void Timer::stop()
    {
        on_ = false;
        restartCounter();
    };

    void Timer::update()
    {
        bool hit = false;

        if ( on() )
        {
            if ( countsDown() )
            {
                if ( counter_ <= limit_ )
                {
                    hit = true;
                }
                else
                {
                    hit = false;
                }
            }
            else
            {
                if ( counter_ >= limit_ )
                {
                    hit = true;
                }
                else
                {
                    hit = false;
                }
            }
        }

        event( hit );
        tick();
    };

    bool Timer::on()      const { return on_; };
    int  Timer::counter() const { return counter_; };

    void Timer::tick()
    {
        if ( on() )
        {
            if ( countsDown() )
            {
                --counter_;
            }
            else
            {
                ++counter_;
            }
        }
    };

    void Timer::restartCounter()
    {
        counter_ = starts_on_;
    };

    bool Timer::countsDown()
    {
        return direction_ == Direction::Vertical::DOWN;
    };
