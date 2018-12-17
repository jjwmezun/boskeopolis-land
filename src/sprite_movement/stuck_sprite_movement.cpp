#include "collision.hpp"
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
	sprite.changeMovement( Type::GROUNDED );
	Sprite::getMovement( Type::GROUNDED )->jump( sprite );
};

void StuckSpriteMovement::bounce( Sprite& sprite, int amount ) const
{
};

void StuckSpriteMovement::position( Sprite& sprite ) const
{
	sprite.vx_ = 0;
	sprite.vy_ = 0;
};

void StuckSpriteMovement::collideStopYBottom( Sprite& sprite, int overlap ) const
{
};

void StuckSpriteMovement::collideStopYTop( Sprite& sprite, int overlap ) const
{
};

void StuckSpriteMovement::collideStopAny( Sprite& sprite, const Collision& collision ) const
{
};

const Collision StuckSpriteMovement::testCollision( const Sprite& me, const sdl2::SDLRect& them ) const
{
};
