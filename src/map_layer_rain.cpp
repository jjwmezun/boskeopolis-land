#include "game.hpp"
#include "level.hpp"
#include "map.hpp"
#include "map_layer_rain.hpp"
#include "math.hpp"
#include "sprite.hpp"
#include "sprite_system.hpp"

namespace BSL
{
    MapLayerRain::MapLayerRain( unsigned int rain_start, unsigned int rain_max )
    :
        drop_showing_ ( 0 ),
        rain_start_ ( static_cast<float> ( blocksToPixels( rain_start ) ) ),
        rain_range_ ( static_cast<float> ( blocksToPixels( rain_max ) ) - rain_start )
    {
        for ( auto & active : active_ )
        {
            active = false;
        }
    };

    void MapLayerRain::update( Level & level, const Game & game, float dt )
    {
        const unsigned int show_drops = static_cast<unsigned int> ( std::min( rain_range_, std::max( 0.0f, level.getPos().x - rain_start_ ) ) / rain_range_ * static_cast<float> ( NUM_O_DROPS ) );

        const float bottom = static_cast<float>( WINDOW_HEIGHT_PIXELS );
        for ( unsigned int i = 0; i < drops_.size(); ++i )
        {
            auto & drop = drops_[ i ];
            drop.addToY( dt );
            if ( drop.getY() > bottom )
            {
                bool prev_active = active_[ i ];
                active_[ i ] = show_drops > drop_showing_;
                if ( active_[ i ] )
                {
                    const float h = static_cast<float>( Math::randInt( 200, 32 ) );
                    const float y = -h - static_cast<float>( Math::randInt( 200 ) );
                    drop.setY( y );
                    drop.setH( h );
                    if ( prev_active )
                    {
                        x_available_.emplace_back( drop.getX() );
                    }
                    else
                    {
                        ++drop_showing_;
                    }
                    Math::shuffle( x_available_ );
                    drop.setX( x_available_.back() );
                    x_available_.pop_back();
                }
                else if ( prev_active )
                {
                    x_available_.emplace_back( drop.getX() );
                    --drop_showing_;
                }
            }
        }
    };

    void MapLayerRain::init( const Game & game )
    {
        for ( unsigned int i = 0; i < NUM_O_DROPS; ++i )
        {
            x_available_.emplace_back( static_cast<float>( i ) * 2.0f );
            const float x = static_cast<float> ( i * 2 );
            const float h = static_cast<float>( Math::randInt( 200, 32 ) );
            const float y = static_cast<float>( WINDOW_HEIGHT_PIXELS );
            drops_[ i ] = game.render().addRectGradient
            (
                x,
                y,
                1.0f,
                h,
                Dir::XY::DOWN,
                64,
                32,
                1.0f,
                1.0f,
                Layer::FG_1,
                0.5f
            );
        }
    };
}