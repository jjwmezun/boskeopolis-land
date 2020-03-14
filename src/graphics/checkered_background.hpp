#pragma once

#include "mezun_sdl2.hpp"

class CheckeredBackground final
{
    public:
        CheckeredBackground();
        void update();
        void render() const;

    private:
        int timer_;
		sdl2::SDLRect screen_;
		sdl2::SDLRect position_;
};