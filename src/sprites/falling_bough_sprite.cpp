#include "collision.hpp"
#include "falling_bough_sprite.hpp"
#include "sprite_graphics.hpp"

FallingBoughSprite::FallingBoughSprite( int x, int y, Direction::Horizontal direction, bool fall )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/woods.png", 104, 32, ( direction == Direction::Horizontal::RIGHT ) ), ( direction == Direction::Horizontal::RIGHT ) ? x : x - 8, y, 24, 8, {}, 10, 1400, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
	fall_ ( fall ),
	falling_timer_ ()
{};
FallingBoughSprite::~FallingBoughSprite() {};

void FallingBoughSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( falling_timer_.on() )
	{
		falling_timer_.update();
	}
	if ( falling_timer_.done() )
	{
		changeMovement( SpriteMovement::Type::GROUNDED );
	}
	hit_box_.x = original_hit_box_.x;
};

void FallingBoughSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
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
