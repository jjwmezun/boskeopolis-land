#pragma once

#include "char_frame.hpp"
#include <vector>

class WTextLine
{
    public:
        void render() const;
        void renderWithoutShadow() const;
        void renderWithShadow() const;
        void renderChar( const CharFrame character, int x, int y ) const;
        void changeColorOffset( int offset );

        std::vector<CharFrame> frames_;
        std::vector<CharFrame> shadow_frames_;
        int x_;
        int y_;
};