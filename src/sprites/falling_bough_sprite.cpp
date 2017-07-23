#include "collision.h"
#include "falling_bough_sprite.h"
#include "sprite_graphics.h"

FallingBoughSprite::FallingBoughSprite( int x, int y, Direction::Horizontal direction, bool fall )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/woods.png", 104, 32 ), x, y, 24, 8, {}, 10, 1400, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
	fall_ ( fall ),
	falling_timer_ ( 8, false )
{
	switch ( direction )
	{
		case ( Direction::Horizontal::RIGHT ):
			graphics_->flip_x_ = true;
		break;

		default:
			hit_box_.x -= 8000;
		break;
	}
};

FallingBoughSprite::~FallingBoughSprite() {};

void FallingBoughSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
{
	if ( falling_timer_.on() )
	{
		falling_timer_.update();
	}

	if ( falling_timer_.done() )
	{
		changeMovement( SpriteMovement::Type::GROUNDED );
	}
};

void FallingBoughSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
{
	if ( them.hasType( Sprite::SpriteType::HERO ) )
	{
		if ( their_collision.collideBottom() && them.prevBottomSubPixels() <= topSubPixels() + 1000  )
		{
			them.collideStopYBottom( their_collision.overlapYBottom() );
			them.addToY( vy_ );

			if ( fall_ && !falling_timer_.on() )
			{
				falling_timer_.start();
			}
		}
	}
};

void FallingBoughSprite::reset()
{
	hit_box_.y = original_hit_box_.y;
	changeMovement( SpriteMovement::Type::FLOATING );
	vy_ = 0;
	acceleration_y_ = 0;
	falling_timer_.stop();
};