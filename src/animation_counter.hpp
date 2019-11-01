#pragma once

#include "timer_repeat_t2.hpp"
#include "unit.hpp"

template <int max_T, int min_T = 0>
class AnimationCounter
{
    public:
        AnimationCounter()
        :
            timer_ (),
            counter_ ( min_T )
        {};

        void update()
        {
            if ( timer_.update() )
            {
                ++counter_;
                if ( counter_ >= max_T )
                {
                    counter_ = min_T;
                }
            }
        }

        int get() const
        {
            return counter_;
        }

    private:
        TimerRepeatT2<Unit::DEFAULT_ANIMATION_SPEED> timer_;
        int counter_;
};