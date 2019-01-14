#pragma once

#include "grounded_sprite_movement.hpp"

class SwimmingSpriteMovement : public GroundedSpriteMovement
{
	public:
		SwimmingSpriteMovement();

		void jump( Sprite& sprite ) const override;
		void position( Sprite& sprite ) const override;
		void bounce( Sprite& sprite, int amount ) const override;

	private:
		static constexpr int SWIM_START_SPEED = 5000;
};
