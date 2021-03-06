#include "cactooie_spine_sprite.hpp"
#include "sprite_graphics.hpp"

CactooieSpineSprite::CactooieSpineSprite( int x, int y, Direction::Horizontal direction )
:
	Sprite
	(
		std::make_unique<SpriteGraphics> ( "sprites/cactooie.png", 72, 0 ),
		x,
		y,
		8,
		8,
		{ SpriteType::ENEMY },
		3000,
		3000,
		0,
		0,
		direction,
		Direction::Vertical::__NULL,
		nullptr,
		SpriteMovement::Type::FLOATING,
		CameraMovement::DESPAWN_OFFSCREEN,
		true,
		false
	)
{};

CactooieSpineSprite::~CactooieSpineSprite() {};

void CactooieSpineSprite::customUpdate( LevelState& level_state )
{
	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::LEFT ):
		{
			moveLeft();
			graphics_->flip_x_ = false;
		}
		break;
			
		case ( Direction::Horizontal::RIGHT ):
		{
			moveRight();
			graphics_->flip_x_ = true;
		}
		break;
	}
};

void CactooieSpineSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};
