#pragma once

#include "sprite.hpp"

class TallTombstoneSprite final : public Sprite
{
	public:
		TallTombstoneSprite( int x, int y );
		~TallTombstoneSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
