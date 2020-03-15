#pragma once

#include "image_graphics.hpp"
#include "mezun_sdl2.hpp"
#include <string>

class ScrollingBackground final
{
    public:
        ScrollingBackground( std::string texture, int width, int height, int speed = 4 );
        void update();
        void render() const;

    private:
        int timer_;
        int speed_;
        int width_;
        int height_;
        ImageGraphics graphics_;
};