#pragma once

#include "mezun_sdl2.hpp"

class Frame final
{
    public:
        constexpr Frame( int x, int y, int w, int h, int color )
        :
            color_ ( color ),
            coords_ ( x, y, w, h )
        {};

        void render() const;

        constexpr void setHeight( int value )
        {
            coords_.h = value;
        };

        constexpr int getHeight() const
        {
            return coords_.h;
        };

        constexpr void changeHeight( int change )
        {
            coords_.h += change;
        };

    private:
        int color_;
        sdl2::SDLRect coords_;
};