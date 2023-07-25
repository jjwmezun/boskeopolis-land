#include "game.hpp"
#include "map_layer_constellation.hpp"
#include "math.hpp"
#include "nasringine/nasr.h"
#include "renderer.hpp"
#include <vector>

namespace BSL
{
    MapLayerConstellation::MapLayerConstellation( unsigned int width, unsigned int height, float scroll_speed )
    :
        width_ ( width ),
        height_ ( height ),
        scroll_speed_ ( scroll_speed ),
        graphics_ ( 0 )
    {};

    void MapLayerConstellation::update( Level & level, const Game & game, float dt )
    {
    };

    void MapLayerConstellation::init( const Game & game )
    {
        std::vector<NasrTile> stars;

        for ( unsigned int i = 0; i < width_ * height_; ++i )
        {
            NasrTile tile
            {
                255,
                255,
                255,
                255
            };

            unsigned int out = static_cast<unsigned int> ( Math::randInt( 11, 0 ) );
            if ( out > 7 )
            {
                tile =
                {
                    0,
                    out == 11 ? 1 : 0,
                    0,
                    out > 9 ? 10 : 0
                };
            }

            stars.push_back( tile );
        }

        game.render().addTilemap
        (
            "constellation",
            stars,
            width_,
            height_,
            { { "opacity", 0.5f }, { "scrollx", 0.9f } }
        );
    };
}