#pragma once

#include "sprite.hpp"

class FallingSnowBoulderSprite : public Sprite
{
	public:
		FallingSnowBoulderSprite( int x, int y );
		~FallingSnowBoulderSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;

	private:
		int timer_;
};
