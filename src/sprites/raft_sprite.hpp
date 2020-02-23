#pragma once

#include "sprite.hpp"

class RaftSprite : public Sprite
{
	public:
		RaftSprite( int x, int y );
		~RaftSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
