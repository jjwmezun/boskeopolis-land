#pragma once

#include "counter_flip.hpp"
#include "image_graphics.hpp"
#include "timer_repeat_t2.hpp"

class ShopPlayer
{
    public:
        ShopPlayer();
        void moveRight();
        void moveLeft();
        void render() const;
        bool testIsAtShopKeeper() const;
        bool hasLeftStore() const;
        void updateGraphics();

    private:
        static constexpr int NUMBER_OF_WALK_FRAMES = 4;
        static constexpr int WALK_FRAMES[ NUMBER_OF_WALK_FRAMES ] =
        {
            0, 58, 0, 29
        };

        bool isMoving() const;
        bool testIsWalking() const;

		TimerRepeatT2<8> animation_timer_;
		CounterFlip<NUMBER_OF_WALK_FRAMES - 1> walk_counter_;
		CounterFlip<11> blink_counter_;
        ImageGraphics graphics_;
        double position_;
        double speed_;
        SDL_RendererFlip flip_;
        char scratch_[3000];
};