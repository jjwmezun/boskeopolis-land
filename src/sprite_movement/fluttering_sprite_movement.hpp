#pragma once

#include "grounded_sprite_movement.hpp"

class FlutteringSpriteMovement final : public GroundedSpriteMovement
{
	public:
		FlutteringSpriteMovement();

		void jump( Sprite& sprite ) const;
		void position( Sprite& sprite ) const;
};