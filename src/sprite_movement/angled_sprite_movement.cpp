#include "sprite.hpp"
#include "angled_sprite_movement.hpp"
#include <iostream>

void angledCollision( Sprite& sprite, int overlap );
void angledCollision( Sprite& sprite, int overlap )
{
	if ( overlap )
	{
		sprite.hit_box_.x = sprite.x_prev_;
		sprite.hit_box_.y = sprite.y_prev_;
		//sprite.speed_ = -sprite.speed_;
	}
};

AngledSpriteMovement::AngledSpriteMovement()
:
	SpriteMovement( Type::ANGLED )
{};

void AngledSpriteMovement::position( Sprite& sprite ) const
{
};

void AngledSpriteMovement::collideStopXLeft( Sprite& sprite, int overlap ) const
{
	sprite.collide_left_ = true;
};

void AngledSpriteMovement::collideStopXRight( Sprite& sprite, int overlap ) const
{
	sprite.collide_right_ = true;
};

void AngledSpriteMovement::collideStopYBottom( Sprite& sprite, int overlap ) const
{
	sprite.collide_bottom_ = true;
};

void AngledSpriteMovement::collideStopYTop( Sprite& sprite, int overlap ) const
{
	sprite.collide_top_ = true;
};