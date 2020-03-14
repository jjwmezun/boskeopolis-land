#pragma once

#include "sprite.hpp"

class FallingSnowBoulderSpawnSprite final : public Sprite
{
	public:
		FallingSnowBoulderSpawnSprite( int x, int y );
		~FallingSnowBoulderSpawnSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		int timer_;
};
