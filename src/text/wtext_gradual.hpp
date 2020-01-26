#pragma once

#include "timer_repeat_t2.hpp"
#include "wtext_obj.hpp"

class WTextGradual
{
    public:
        WTextGradual( WTextObj text );
        void update();
        void render() const;

    private:
        TimerRepeatT2<2> timer_;
        int characters_in_view_;
        int limit_;
        WTextObj text_;
};