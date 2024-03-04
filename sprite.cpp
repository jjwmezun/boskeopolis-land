#include "config.hpp"
#include <stdexcept>
#include "sprite.hpp"

namespace BSL
{
    enum class SpriteTile
    {
        PLAYER = 1,
        CRAB   = 2,
        APPLE_LEFT = 3,
        APPLE_RIGHT = 4
    };

    void Sprite::update( float dt, Level & level )
    {
        switch ( type )
        {
            case ( SpriteType::CRAB ):
            {
                updateCrab( *this, dt, level );
            }
            break;
            case ( SpriteType::APPLE ):
            {
                updateApple( *this, dt, level );
            }
            break;
        }
    };

    void Sprite::init( uint_fast16_t index, uint_fast16_t x, uint_fast16_t y )
    {
        const SpriteTile type = static_cast<SpriteTile> ( index );
        const uint_fast16_t xpixels = x * BLOCK_SIZE;
        const uint_fast16_t ypixels = y * BLOCK_SIZE;
        switch ( type )
        {
            case ( SpriteTile::CRAB ):
            {
                initCrab( *this, xpixels, ypixels );
            }
            break;
            case ( SpriteTile::APPLE_LEFT ):
            {
                initApple( *this, xpixels, ypixels, DirX::LEFT );
            }
            break;
            case ( SpriteTile::APPLE_RIGHT ):
            {
                initApple( *this, xpixels, ypixels, DirX::RIGHT );
            }
            break;
            default:
            {
                throw std::runtime_error( "Sprite ID “" + std::to_string( index ) + "” out o’ range." );
            }
        }
    };
};