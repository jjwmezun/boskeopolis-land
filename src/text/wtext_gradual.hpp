#pragma once

#include "audio.hpp"
#include "timer_repeat_t2.hpp"
#include "wtext_obj.hpp"

template<bool play_sound_ = true>
class WTextGradual final
{
    public:
        WTextGradual( WTextObj text )
        :
            timer_ (),
            characters_in_view_ ( 0 ),
            limit_ ( text.getNumberOfCharacters() ),
            text_ ( text )
        {};

        void update()
        {
            if ( characters_in_view_ < limit_ )
            {
                ++characters_in_view_;
                if ( play_sound_ )
                {
                    Audio::playSound( Audio::SoundType::SELECT );
                }
            }
        };

        void render() const
        {
            if ( characters_in_view_ < limit_ )
            {
                text_.render( characters_in_view_ );
            }
            else
            {
                text_.render();
            }
        };

        void reset()
        {
            characters_in_view_ = 0;
        };

        bool finishedAppearing() const
        {
            return characters_in_view_ >= limit_;
        };

        const WTextObj& getText() const
        {
            return text_;
        };

    private:
        TimerRepeatT2<2> timer_;
        int characters_in_view_;
        int limit_;
        WTextObj text_;
};