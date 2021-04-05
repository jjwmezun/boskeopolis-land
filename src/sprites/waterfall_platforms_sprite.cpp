#include "waterfall_platforms_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "map.hpp"
#include "sprite_graphics.hpp"

WaterfallPlatformsSprite::WaterfallPlatformsSprite( int x, int y, Direction::Vertical direction, int space_between_platforms, int speed )
:
	Sprite
    (
        std::make_unique<SpriteGraphics> ( "tilesets/woods.png", 96, 94, false, false, 0.0, 0, -2, 0, 2 ),
        x,
        ( direction == Direction::Vertical::DOWN ) ? Unit::BlocksToPixels( 1000 + ( Unit::PixelsToBlocks( y ) % space_between_platforms ) ) : 0,
        32,
        5,
        {},
        speed,
        speed,
        0,
        0,
        Direction::Horizontal::__NULL,
        direction,
        nullptr,
        SpriteMovement::Type::FLOATING,
        CameraMovement::PERMANENT
    ),
    space_between_platforms_ ( Unit::BlocksToSubPixels( space_between_platforms ) )
{
    for ( int i = 0; i < MAX_PLATFORMS; ++i )
    {
        platforms_[ i ] = { hit_box_.x, hit_box_.y, hit_box_.w, hit_box_.h };
        platforms_on_camera_[ i ] = false;
    }
};

WaterfallPlatformsSprite::~WaterfallPlatformsSprite() {};

void WaterfallPlatformsSprite::customUpdate( LevelState& level_state )
{
    const Camera& camera = level_state.camera();
    switch ( direction_y_ )
    {
        case ( Direction::Vertical::UP ):
        {
            moveUp();
            if ( hit_box_.y < -space_between_platforms_ )
            {
                hit_box_.y += space_between_platforms_;
            }

            int y = hit_box_.y;
            while ( y < Unit::PixelsToSubPixels( camera.y() ) - space_between_platforms_ )
            {
                y += space_between_platforms_;
            }

            for ( int i = 0; i < MAX_PLATFORMS; ++i )
            {
                platforms_[ i ].y = y;
                y += space_between_platforms_;
            }
        }
        break;
        case ( Direction::Vertical::DOWN ):
        {
            moveDown();
            if ( hit_box_.y > Unit::BlocksToSubPixels( level_state.currentMap().heightBlocks() ) + space_between_platforms_ )
            {
                hit_box_.y -= space_between_platforms_;
            }

            int y = hit_box_.y;
            while ( y > Unit::PixelsToSubPixels( camera.bottom() ) + space_between_platforms_ )
            {
                y -= space_between_platforms_;
            }

            for ( int i = 0; i < MAX_PLATFORMS; ++i )
            {
                platforms_[ i ].y = y;
                y -= space_between_platforms_;
            }
        }
        break;
    }
};

void WaterfallPlatformsSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
        const bool blocks_in_the_way = ( direction_y_ == Direction::Vertical::DOWN ) && level_state.blocks().blocksInTheWay
        (
            {
                them.hit_box_.x,
                them.bottomSubPixels(),
                them.rightSubPixels(),
                1000
            },
            BlockComponent::Type::SOLID
        );
        
        for ( int i = 0; i < MAX_PLATFORMS; ++i )
        {
            const Collision collision = movement_->testBlockCollision( them, platforms_[ i ] );
            const bool condition = ( direction_y_ == Direction::Vertical::DOWN ) ? them.prevBottomSubPixels() <= platforms_[ i ].y + 1000 : them.prevBottomSubPixels() <= platforms_[ i ].y + 2000;
            if ( collision.collideBottom() && condition )
            {
                if ( !blocks_in_the_way )
                {
                    them.hit_box_.y += vy_;
                }
                them.collideStopYBottom( collision.overlapYBottom() );
            }
        }
	}
};

void WaterfallPlatformsSprite::render( const Camera& camera ) const
{
    for ( int i = 0; i < MAX_PLATFORMS; ++i )
    {
        graphics_->render( Unit::SubPixelsToPixels( platforms_[ i ] ), &camera );
    }
};
