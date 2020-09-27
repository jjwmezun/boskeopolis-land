#pragma once

#include "sprite.hpp"

class DoomEnemySprite final : public Sprite
{
	public:
		DoomEnemySprite( int x, int y );
		~DoomEnemySprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void deathAction( LevelState& level_state );

	private:
		int animation_;
};
