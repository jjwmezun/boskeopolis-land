#pragma once

#include "wtext_character.hpp"
#include <vector>

class WTextLine
{
    public:
        inline WTextLine( int x, int y, std::vector<WTextCharacter> frames, std::vector<WTextCharacter> shadow_frames )
        :
            x_ ( x ),
            y_ ( y ),
            frames_ ( frames ),
            shadow_frames_ ( shadow_frames )
        {};
        void render() const;
        void renderWithoutShadow() const;
        void renderWithShadow() const;
        void render( int limit ) const;
        void renderWithoutShadow( int limit ) const;
        void renderWithShadow( int limit ) const;
        void renderChar( const WTextCharacter character, int x, int y ) const;
        void changeColorOffset( int offset );

        int x_;
        int y_;
        std::vector<WTextCharacter> frames_;
        std::vector<WTextCharacter> shadow_frames_;
};