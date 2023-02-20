#include "config.hpp"
#include "game.hpp"
#include "level.hpp"
#include "map.hpp"
#include "map_layer_palette_change.hpp"
#include "math.hpp"
#include "sprite.hpp"
#include "sprite_system.hpp"

namespace BSL
{
    MapLayerPaletteChange::MapLayerPaletteChange( unsigned int start, unsigned int max )
    :
        start_ ( static_cast<float> ( blocksToPixels( start ) ) ),
        range_ ( static_cast<float> ( blocksToPixels( max ) ) - start )
    {};

    void MapLayerPaletteChange::update( Level & level, const Game & game, float dt )
    {
        game.render().setPalette( static_cast<uint_fast8_t> ( std::floor( std::min( range_, std::max( 0.0f, level.sprites().getHero().getPos().x - start_ ) ) / range_ * 255.0f ) ) );
    };

    void MapLayerPaletteChange::init( const Game & game )
    {
        NasrSetPalette( "assets/palettes/city-sunrise-palette.png" );
    };
}