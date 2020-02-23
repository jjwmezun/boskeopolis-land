#pragma once

class Block;

#include "sprite.hpp"

class RollingBallSprite : public Sprite
{
	public:
		RollingBallSprite( int x, int y );
		~RollingBallSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};