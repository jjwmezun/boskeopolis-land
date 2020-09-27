#pragma once

#include "sprite.hpp"

class FallingSnowBoulderSprite final : public Sprite
{
	public:
		FallingSnowBoulderSprite( int x, int y );
		~FallingSnowBoulderSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void deathAction( LevelState& level_state ) override;

	private:
		int timer_;
};
