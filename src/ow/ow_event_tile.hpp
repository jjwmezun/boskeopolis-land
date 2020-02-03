#pragma once

class OWEventTile
{
    public:
        enum class Layer
        {
            BG,
            FG,
            SPRITE
        };

        constexpr OWEventTile( Layer layer, int position, int tile )
        :
            layer_ ( layer ),
            position_ ( position ),
            tile_ ( tile )
        {};

        const Layer layer_;
        const int position_;
        const int tile_;
};