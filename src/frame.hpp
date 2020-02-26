#pragma once

#include "mezun_sdl2.hpp"

class Frame
{
    public:
        Frame( int x, int y, int w, int h, int color = 1 );
        void render() const;

    private:
        int color_;
        sdl2::SDLRect coords_;
};