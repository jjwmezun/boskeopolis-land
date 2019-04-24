#include "top_down_moveable_block_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

TopDownMoveableBlockSprite::TopDownMoveableBlockSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/universal.png", 112, 0 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

TopDownMoveableBlockSprite::~TopDownMoveableBlockSprite() {};

void TopDownMoveableBlockSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{};

void TopDownMoveableBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		const bool extra_condition = them.hit_box_.x < rightSubPixels() - 2000 &&
									 them.rightSubPixels() > hit_box_.x + 2000 &&
									 them.hit_box_.y < bottomSubPixels() - 2000 &&
									 them.bottomSubPixels() > hit_box_.y + 2000;
		if ( extra_condition )
		{
			hit_box_.x += them.vx_ / 2;
			hit_box_.y += them.vy_ / 2;
		}
		them.collideStopAny( their_collision );
	}
};
