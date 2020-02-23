#pragma once

#include "sprite.hpp"

class DoomEnemySprite : public Sprite
{
	public:
		DoomEnemySprite( int x, int y );
		~DoomEnemySprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap );

	private:
		int animation_;
};
