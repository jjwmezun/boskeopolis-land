



// Name
//===================================
//
// BouncyCloudBlockSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "bouncy_cloud_block_sprite.h"
	#include "map.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BouncyCloudBlockSprite::BouncyCloudBlockSprite( int x, int y )
    :
        Sprite
		(
			std::make_unique<SpriteGraphics>
			(
				"sprites/cloud_block.png",
				64
			),
			x,
			y,
			16,
			16,
			{},
			100,
			100,
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

    BouncyCloudBlockSprite::~BouncyCloudBlockSprite() {};

    void BouncyCloudBlockSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
		if ( hit_box_.y < original_hit_box_.y + 99 || hit_box_.y > original_hit_box_.y - 99 )
		{
			fullStopY();
			hit_box_.y = original_hit_box_.y;
		}
		else if ( hit_box_.y > original_hit_box_.y )
		{
			moveUp();
		}
		else if ( hit_box_.y < original_hit_box_.y )
		{
			moveDown();
		}
    };

    void BouncyCloudBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
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

			if ( their_collision.collideBottom() )
			{
				hit_box_.y += them.vy_;	
				them.changeY( hit_box_.y - them.heightSubPixels() );
				them.bounce( ( hit_box_.y - original_hit_box_.y ) / 300 );
			}
			
		}
    };
