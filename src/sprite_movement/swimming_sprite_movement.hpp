#pragma once

#include "grounded_sprite_movement.hpp"

class SwimmingSpriteMovement : public GroundedSpriteMovement
{
	public:
		SwimmingSpriteMovement();

		void jump( Sprite& sprite ) const;
		void position( Sprite& sprite ) const;

	private:
		static constexpr int SWIM_START_SPEED = 5000;
};