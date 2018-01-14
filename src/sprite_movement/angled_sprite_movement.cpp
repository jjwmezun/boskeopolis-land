#include "sprite.hpp"
#include "angled_sprite_movement.hpp"
#include <iostream>

AngledSpriteMovement::AngledSpriteMovement()
:
	SpriteMovement( Type::ANGLED )
{};

void AngledSpriteMovement::position( Sprite& sprite ) const
{
};

void AngledSpriteMovement::collideStopXLeft( Sprite& sprite, int overlap ) const
{
	//sprite.hit_box_.x += overlap;
};

void AngledSpriteMovement::collideStopXRight( Sprite& sprite, int overlap ) const
{
	//sprite.hit_box_.x -= overlap;
};

void AngledSpriteMovement::collideStopYBottom( Sprite& sprite, int overlap ) const
{
	//sprite.hit_box_.y -= overlap;
};

void AngledSpriteMovement::collideStopYTop( Sprite& sprite, int overlap ) const
{
	//sprite.hit_box_.y += overlap;
};