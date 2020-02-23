#pragma once

class Block;

#include "player_sprite.hpp"

class RacerSprite : public PlayerSprite
{
	public:
		RacerSprite( int x, int y );
		~RacerSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
};
