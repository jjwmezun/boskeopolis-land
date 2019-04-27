#include "top_down_moveable_block_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

// Note: top_speed_ used as hole trigger 'cause polymorphism is too limited for using sane variable names.

TopDownMoveableBlockSprite::TopDownMoveableBlockSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/moveable-block.png", 0, 0 ), x, y, 16, 16, { SpriteType::MOVEABLE_BLOCK }, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

TopDownMoveableBlockSprite::~TopDownMoveableBlockSprite() {};

void TopDownMoveableBlockSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{};

void TopDownMoveableBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( top_speed_ == 0 && them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		const bool x_condition = ( them.hit_box_.y < bottomSubPixels() - 4000 &&
								   them.bottomSubPixels() > hit_box_.y + 4000 );
		if ( x_condition )
		{
			hit_box_.x += them.vx_ / 2;
		}
		else
		{
			const bool y_condition = ( them.hit_box_.x < rightSubPixels() - 4000 &&
								       them.rightSubPixels() > hit_box_.x + 4000 );
			if ( y_condition )
			{
				hit_box_.y += them.vy_ / 2;
			}
		}

		them.collideStopAny( their_collision );
	}
};

void TopDownMoveableBlockSprite::reset()
{
	if ( top_speed_ == 0 )
	{
		hit_box_ = original_hit_box_;
	}
}