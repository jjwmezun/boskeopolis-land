#pragma once

#include "circle_movement.hpp"
#include "sprite.hpp"

class CraneCrateSprite final : public Sprite
{
	public:
		CraneCrateSprite( int x, int y );
		~CraneCrateSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( Camera& camera, bool priority ) override;

	private:
		CircleMovement circle_;
		int prev_x_;
		int prev_y_;
};
