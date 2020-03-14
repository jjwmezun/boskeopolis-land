#pragma once

#include "timer_t.hpp"

template <int limit_T = TIMER_T_DEFAULT_FRAMES, bool starts_on_T = true, int start_count_T = 0, Direction::Vertical direction_T = Direction::Vertical::UP>
class TimerRepeatT final : public TimerT<limit_T, starts_on_T, start_count_T, direction_T>
{
    public:
        constexpr TimerRepeatT()
        :
            TimerT<limit_T, starts_on_T, start_count_T, direction_T> (),
        	hit_ ( false ),
        	num_o_hits_ ( 0 )
        {};
        ~TimerRepeatT() {};

        void start()
        {
            this->on_ = true;
            this->restartCounter();
            this->hit_ = false;
        };

        void stop()
        {
            this->on_ = false;
            this->restartCounter();
            this->hit_ = false;
        };

        void event( bool hit )
        {
            if ( hit )
            {
                this->hit_ = true;
        		++this->num_o_hits_;
                this->restartCounter();
            }
            else
            {
                this->hit_ = false;
            }
        };

        constexpr bool hit() const { return this->hit_; };
        constexpr int numOHits() const { return this->num_o_hits_; };

    protected:
        bool hit_;
		int num_o_hits_;
};
