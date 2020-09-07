#pragma once

#include "sprite.hpp"

class FallingStarSprite final : public Sprite
{
	public:
		FallingStarSprite( int x, int y );
		~FallingStarSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        char scratch_[ 3000 ];
};
