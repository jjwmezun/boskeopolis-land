#pragma once

#include "wtext_character.hpp"
#include <vector>

class WTextLine
{
    public:
        void render() const;
        void renderWithoutShadow() const;
        void renderWithShadow() const;
        void renderChar( const WTextCharacter character, int x, int y ) const;
        void changeColorOffset( int offset );

        std::vector<WTextCharacter> frames_;
        std::vector<WTextCharacter> shadow_frames_;
        int x_;
        int y_;
};