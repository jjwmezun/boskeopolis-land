#include "map.hpp"
#include <memory>
#include "mezun_math.hpp"
#include "pedestrian_sprite.hpp"
#include "pedestrian_generator_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

PedestrianGeneratorSprite::PedestrianGeneratorSprite( int x, int y )
:
	Sprite( nullptr, x, y, 1, 1, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

PedestrianGeneratorSprite::~PedestrianGeneratorSprite() {};

void PedestrianGeneratorSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
    for ( int y = 0; y < lvmap.heightBlocks(); ++y )
    {
        for ( int x = 0; x < lvmap.widthBlocks(); ++x )
        {
            const int n = lvmap.indexFromXAndY( x, y );
            const int block_type = lvmap.block( n );
            if ( block_type == 66 && mezun::testRandomWithinPercentage( 10 ) )
            {
                const int xpixels = Unit::BlocksToPixels( x );
                const int ypixels = Unit::BlocksToPixels( y );
                sprites.spawn( std::make_unique<PedestrianSprite> ( xpixels, ypixels ) );
            }
        }
    }
    killNoAnimation();
};

void PedestrianGeneratorSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) {};
