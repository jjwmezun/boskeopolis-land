#pragma once

class Block;

#include "sprite.hpp"

class BatSprite : public Sprite
{
	public:
		BatSprite( int x, int y );
		~BatSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		
		private:
			bool awake_;
};