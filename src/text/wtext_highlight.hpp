#pragma once

#include "wtext_obj.hpp"

static constexpr int NO_CHARACTER_HIGHLIGHTED = -1;

template<int speed = 4, int delay = 60>
class WTextHighlight
{
    public:
        WTextHighlight( WTextObj text )
        :
            timer_ (),
            current_character_ ( -1 ),
            text_ ( text )
        {};

        void update()
        {
            if ( current_character_ == NO_CHARACTER_HIGHLIGHTED )
            {
                if ( timer_ == delay )
                {
                    ++current_character_;
                    timer_ = 0;
                    updateTitleHighlightGraphics();
                }
                else
                {
                    ++timer_;
                }
            }
            else
            {
                if ( timer_ == speed )
                {
                    ++current_character_;
                    timer_ = 0;
                    const int number_of_characters = ( int )( text_.lines_[ 0 ].frames_.size() );
                    if ( current_character_ == number_of_characters )
                    {
                        current_character_ = NO_CHARACTER_HIGHLIGHTED;
                    }
                    updateTitleHighlightGraphics();
                }
                else
                {
                    ++timer_;
                }
            }
        };

        void render() const
        {
            text_.render();
        };

        const WTextObj& getText() const
        {
            return text_;
        };

    private:
        void updateTitleHighlightGraphics()
        {
            // If 1st character, we only need to highlight the 1st character.
            if ( current_character_ == 0 )
            {
                text_.lines_[ 0 ].frames_[ 0 ].changeColor( WTextCharacter::Color::WHITE );
            }
            // If last character, we only need to unhighlight last character.
            else if ( current_character_ == NO_CHARACTER_HIGHLIGHTED )
            {
                text_.lines_[ 0 ].frames_[ text_.lines_[ 0 ].frames_.size() - 1 ].changeColor( WTextCharacter::Color::LIGHT_GRAY );
            }
            // Any other character, highlight current character & unhighlight previous.
            else
            {
                text_.lines_[ 0 ].frames_[ current_character_ ].changeColor( WTextCharacter::Color::WHITE );
                text_.lines_[ 0 ].frames_[ current_character_ - 1 ].changeColor( WTextCharacter::Color::LIGHT_GRAY );
            }
        };

        int timer_;
        int current_character_;
        WTextObj text_;
};