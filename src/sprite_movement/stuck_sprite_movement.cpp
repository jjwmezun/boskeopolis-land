#include "collision.hpp"
#include "input.hpp"
#include "sprite.hpp"
#include "stuck_sprite_movement.hpp"

StuckSpriteMovement::StuckSpriteMovement()
:
	SpriteMovement( Type::STUCK )
{};

void StuckSpriteMovement::moveUp( Sprite& sprite ) const
{
};

void StuckSpriteMovement::moveDown( Sprite& sprite ) const
{
};

void StuckSpriteMovement::moveLeft( Sprite& sprite ) const
{
};

void StuckSpriteMovement::moveRight( Sprite& sprite ) const
{
};

void StuckSpriteMovement::jump( Sprite& sprite ) const
{
	if ( Input::pressed( Input::Action::JUMP ) )
	{
		sprite.changeMovement( Type::GROUNDED );
		Sprite::getMovement( Type::GROUNDED )->jump( sprite );
	}
};

void StuckSpriteMovement::bounce( Sprite& sprite, int amount ) const
{
};

void StuckSpriteMovement::position( Sprite& sprite ) const
{
	sprite.on_ground_ = true;
	sprite.vx_ = 0;
	sprite.vy_ = 0;
	sprite.acceleration_y_ = 0;
};

const Collision StuckSpriteMovement::testBlockCollision( const Sprite& me, const sdl2::SDLRect& them ) const
{
};
