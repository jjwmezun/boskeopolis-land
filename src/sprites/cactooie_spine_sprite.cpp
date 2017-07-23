#include "cactooie_spine_sprite.h"
#include "sprite_graphics.h"

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

void CactooieSpineSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
{
	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::LEFT ):
			moveLeft();
			graphics_->flip_x_ = false;
		break;
			
		case ( Direction::Horizontal::RIGHT ):
			moveRight();
			graphics_->flip_x_ = true;
		break;
	}
};

void CactooieSpineSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
{
};
