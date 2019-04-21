#include "anchor_missile_sprite.hpp"
#include "sprite_graphics.hpp"

AnchorMissileSprite::AnchorMissileSprite( int x, int y, Direction::Simple direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/anchor-missile.png", 0, 0, ( ( direction == Direction::Simple::RIGHT ) ? true : false ), 0, ( ( ( direction == Direction::Simple::DOWN ) ? 270 : ( direction == Direction::Simple::UP ) ? 90 : 0 ) ) ), x, y, 16, 16, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 200, 3000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, true )
{
	direction_ = direction;
};

AnchorMissileSprite::~AnchorMissileSprite() {};

void AnchorMissileSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	moveInDirection();
	if ( collide_top_ || collide_left_ || collide_right_ || collide_bottom_ )
	{
		kill();
	}
};

void AnchorMissileSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) {};
