#pragma once

#include "timer.hpp"

class TimerRepeat : public Timer
{
    public:
        TimerRepeat
        (
            int limit = NORMAL_FRAMES,
            bool starts_on = true    ,
            int start_count = 0      ,
            Direction::Vertical direction = Direction::Vertical::UP
        );
        ~TimerRepeat();

        void start();
        void stop();
        void event( bool hit );
        bool hit() const;
		int numOHits() const;

    protected:
        bool hit_;
		int num_o_hits_;
};
