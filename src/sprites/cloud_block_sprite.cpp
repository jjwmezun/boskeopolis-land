



// Name
//===================================
//
// CloudBlockSprite
//


// DEPENDENCIES
//===================================

    #include "collision.hpp"
    #include "cloud_block_sprite.hpp"
	#include "map.hpp"
    #include "sprite_graphics.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    CloudBlockSprite::CloudBlockSprite( int x, int y )
    :
        Sprite
		(
			std::make_unique<SpriteGraphics>
			(
				"sprites/cloud_block.png"
			),
			x,
			y,
			16,
			16,
			{},
			0,
			0,
			0,
			0,
			Direction::Horizontal::__NULL,
			Direction::Vertical::__NULL,
			nullptr,
			SpriteMovement::Type::FLOATING,
			CameraMovement::PERMANENT
		)
    {
    };

    CloudBlockSprite::~CloudBlockSprite() {};

    void CloudBlockSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
		// For map loop
		hit_box_.x = original_hit_box_.x;
    };

    void CloudBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
    {
		if ( them.hasType( SpriteType::HERO ) )
		{
			if ( them.collideBottomOnly( their_collision, *this ) )
			{
				them.collideStopYBottom( their_collision.overlapYBottom() );
			}
			
			if
			(
				them.xSubPixels() > xSubPixels() - Unit::BlocksToSubPixels( 1 ) &&
				them.rightSubPixels() < rightSubPixels() + Unit::BlocksToSubPixels( 1 )
			)
			{
				graphics_->current_frame_x_ = 32;
			}
			else if
			(
				them.xSubPixels() > xSubPixels() - Unit::BlocksToSubPixels( 6 ) &&
				them.rightSubPixels() < rightSubPixels()
			)
			{
				graphics_->current_frame_x_ = 16;
			}
			else if
			(
				them.xSubPixels() > xSubPixels() &&
				them.rightSubPixels() < rightSubPixels() + Unit::BlocksToSubPixels( 6 )
			)
			{
				graphics_->current_frame_x_ = 48;
			}
			else
			{
				graphics_->current_frame_x_ = 0;
			}
			
		}
    };
