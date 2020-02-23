#include "block_system.hpp"
#include "camera.hpp"
#include "level_state.hpp"
#include "moveable_block_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

MoveableBlockSprite::MoveableBlockSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/house.png", 144 ), x, y, 32, 32, {}, 4000, 4000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    current_vx_ ( 0 )
{};

MoveableBlockSprite::~MoveableBlockSprite() {};

void MoveableBlockSprite::customUpdate( LevelState& level_state )
{
    if ( current_vx_ )
    {
        vx_ = current_vx_;
        current_vx_ = 0;
    }
    else
    {
        vx_ = 0;
    }
};

void MoveableBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( their_collision.collideAny() )
	{
        if ( them.hasType( SpriteType::HERO ) )
        {
            BlockSystem& blocks = level_state.blocks();
            const bool x_condition = them.hit_box_.y < bottomSubPixels() - 4000 && them.bottomSubPixels() > hit_box_.y + 4000 &&
                (
                    ( them.rightSubPixels() <= hit_box_.x + 4000 && them.vx_ > 0 && !blocks.blocksInTheWay( getLeftSide(), BlockComponent::Type::SOLID ) ) ||
                    ( them.hit_box_.x >= rightSubPixels() - 4000 && them.vx_ < 0 && !blocks.blocksInTheWay( getRightSide(), BlockComponent::Type::SOLID ) )
                );
            if ( x_condition )
            {
                current_vx_ = ( double )( them.vx_ ) * 2.0;
            }
            them.collideStopAny( their_collision );
        }
        else
        {
            if ( my_collision.collideLeft() )
            {
                movement_->collideStopXLeft( *this, my_collision.overlapXLeft() );
            }
            if ( my_collision.collideRight() )
            {
                movement_->collideStopXRight( *this, my_collision.overlapXRight() );
            }

            if ( my_collision.collideBottom() )
            {
                movement_->collideStopYBottom( *this, my_collision.overlapYBottom() );
            }

            if ( my_collision.collideTop() )
            {
                movement_->collideStopYTop( *this, my_collision.overlapYTop() );
            }
        }
	}
};

sdl2::SDLRect MoveableBlockSprite::getLeftSide() const
{
    return { rightSubPixels() - 4000, hit_box_.y + 4000, 8000, hit_box_.h - 8000 };
};

sdl2::SDLRect MoveableBlockSprite::getRightSide() const
{
    return { hit_box_.x - 4000, hit_box_.y + 4000, 8000, hit_box_.h - 8000 };
};

void MoveableBlockSprite::render( Camera& camera, bool priority )
{
    graphics_->render( Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
    const auto left = camera.relativeRect( Unit::SubPixelsToPixels( getLeftSide() ) );
    Render::renderRectDebug( left, { 255, 0, 0, 128 } );
    const auto right = camera.relativeRect( Unit::SubPixelsToPixels( getRightSide() ) );
    Render::renderRectDebug( right, { 255, 0, 0, 128 } );
};
