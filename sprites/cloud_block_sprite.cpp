



// Name
//===================================
//
// CloudBlockSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "cloud_block_sprite.h"
	#include "map.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    CloudBlockSprite::CloudBlockSprite( int x, int y )
    :
        Sprite
		(
			std::unique_ptr<SpriteGraphics>
			(
				new SpriteGraphics
				(
					Graphics::SpriteSheet::LVSPRITES_CLOUD_BLOCK
				)
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
		),
		loop_ ( 0 )
    {
    };

    CloudBlockSprite::~CloudBlockSprite() {};

    void CloudBlockSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
    };

    void CloudBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
    {
		if ( them.hasType( SpriteType::HERO ) )
		{
			if ( lvmap.scrollLoop() )
			{
				int player_loop = floor( them.xPixels() / lvmap.widthPixels() );

				if ( them.xPixels() < 0 )
					--player_loop;
					
				if ( loop_ != player_loop )
				{
					hit_box_.x = original_hit_box_.x + ( Unit::PixelsToSubPixels( lvmap.widthPixels() ) * player_loop );
					loop_ = player_loop;
				}
			}

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
