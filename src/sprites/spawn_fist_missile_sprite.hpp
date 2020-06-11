#pragma once

#include "sprite.hpp"

class SpawnFistMissileSprite final : public Sprite
{
	public:
		SpawnFistMissileSprite( int x, int y );
		~SpawnFistMissileSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		int timer_;
		int target_time_;
};
