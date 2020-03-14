#pragma once

#include "image_graphics.hpp"
#include "timer_repeat_t2.hpp"

class OWCameraArrows final
{
    public:
        OWCameraArrows();
        void update( const sdl2::SDLRect& camera );
        void render() const;

    private:
        bool testShowArrows( const sdl2::SDLRect& camera ) const;

        bool show_arrows_;
        int prev_camera_x_;
        int prev_camera_y_;
		TimerRepeatT2<48> animation_timer_;
        ImageGraphics graphics_;
};