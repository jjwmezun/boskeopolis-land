#pragma once

#include "sprite.hpp"

class DoomDoorSprite : public Sprite
{
	public:
		DoomDoorSprite( int x, int y );
		~DoomDoorSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
