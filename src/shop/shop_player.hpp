#pragma once

#include "counter_flip.hpp"
#include "image_graphics.hpp"
#include "timer_repeat_t2.hpp"

class ShopPlayer
{
    public:
        ShopPlayer();
        void update();
        void render() const;

    private:
        static constexpr int NUMBER_OF_WALK_FRAMES = 4;
        static constexpr int WALK_FRAMES[ NUMBER_OF_WALK_FRAMES ] =
        {
            0, 32, 0, 16
        };

        void updatePosition();
        void updateGraphics();
        bool isMoving() const;

		TimerRepeatT2<8> animation_timer_;
		CounterFlip<NUMBER_OF_WALK_FRAMES - 1> walk_counter_;
		CounterFlip<11> blink_counter_;
        ImageGraphics graphics_;
        double position_;
        double speed_;
        char scratch_[3000];
};