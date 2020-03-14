#pragma once

class OWEventTile final
{
    public:
        enum class Layer
        {
            BG1,
            BG2,
            FG1,
            FG2,
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