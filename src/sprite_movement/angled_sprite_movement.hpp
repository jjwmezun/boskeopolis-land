#pragma once

#include "sprite_movement.hpp"

class AngledSpriteMovement : public SpriteMovement
{
	public:
		AngledSpriteMovement();
		void position( Sprite& sprite ) const override;
		void collideStopXLeft( Sprite& sprite, int overlap ) const override;
		void collideStopXRight( Sprite& sprite, int overlap ) const override;
		void collideStopYBottom( Sprite& sprite, int overlap ) const override;
		void collideStopYTop( Sprite& sprite, int overlap ) const override;
};