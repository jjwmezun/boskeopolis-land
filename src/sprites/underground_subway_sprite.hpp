#pragma once

class Block;

#include "sprite.hpp"

class UndergroundSubwaySprite : public Sprite
{
	public:
		UndergroundSubwaySprite( int x, int y );
		~UndergroundSubwaySprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		
	private:
		bool activated_;
};