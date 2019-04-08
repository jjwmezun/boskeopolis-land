#pragma once

#include "direction.hpp"

static constexpr int TIMER_T_DEFAULT_FRAMES = 8;

template <int limit_T = TIMER_T_DEFAULT_FRAMES, bool starts_on_T = true, int start_count_T = 0, Direction::Vertical direction_T = Direction::Vertical::UP>
class TimerT
{
    public:
        constexpr TimerT()
        :
            on_ ( starts_on_T ),
            counter_ ( start_count_T )
        {};
        virtual ~TimerT() {};

        constexpr void pause() { on_ = false; };
        constexpr void resume() { on_ = true; };
    	void restart() { start(); };

        virtual void start()
        {
            on_ = true;
            restartCounter();
        };

        virtual void stop()
        {
            on_ = false;
            restartCounter();
        };

        void update()
        {
            bool hit = false;

            if ( on() )
            {
                if ( countsDown() )
                {
                    if ( counter_ <= limit_T )
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
                    if ( counter_ >= limit_T )
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

        void forceUpdate()
    	{
    		on_ = true;
    		update();
    	};

		constexpr double countPercent() const
    	{
    		return ( ( double ) counter_ / ( double ) limit_T );
    	};

        constexpr bool on()     const { return on_; };
        constexpr int counter() const { return counter_; };


    protected:
        void tick()
        {
            if ( on_ )
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

        constexpr void restartCounter()
        {
            counter_ = starts_on_T;
        };

        static constexpr bool countsDown()
        {
            return direction_T == Direction::Vertical::DOWN;
        };

        virtual void event( bool hit ) = 0;


        bool on_;
        int  counter_;
};
