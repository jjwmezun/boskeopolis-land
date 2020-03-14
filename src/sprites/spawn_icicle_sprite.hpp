#pragma once

#include "sprite.hpp"

class SpawnIcicleSprite final : public Sprite
{
	public:
		SpawnIcicleSprite( int x, int y );
		~SpawnIcicleSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		int timer_;
		bool go_;
};
