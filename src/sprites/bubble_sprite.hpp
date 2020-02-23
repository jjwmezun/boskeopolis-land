#pragma once

#include "sprite.hpp"

class BubbleSprite : public Sprite
{
	public:
		BubbleSprite( int x, int y );
		~BubbleSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
