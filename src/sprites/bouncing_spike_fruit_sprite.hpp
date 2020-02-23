#pragma once

#include "sprite.hpp"

class BouncingSpikeFruitSprite : public Sprite
{
	public:
		BouncingSpikeFruitSprite( int x, int y );
		~BouncingSpikeFruitSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
