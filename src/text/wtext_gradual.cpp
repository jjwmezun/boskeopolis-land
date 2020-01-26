#include "audio.hpp"
#include "wtext_gradual.hpp"

WTextGradual::WTextGradual( WTextObj text )
:
    timer_ (),
    characters_in_view_ ( 0 ),
    limit_ ( text.getNumberOfCharacters() ),
    text_ ( text )
{};

void WTextGradual::update()
{
    if ( characters_in_view_ < limit_ )
    {
        ++characters_in_view_;
        Audio::playSound( Audio::SoundType::SELECT );
    }
};

void WTextGradual::render() const
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