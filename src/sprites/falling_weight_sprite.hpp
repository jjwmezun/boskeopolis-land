#pragma once

#include "sprite.hpp"

class FallingWeightSprite final : public Sprite
{
	public:
		FallingWeightSprite( int x, int y );
		~FallingWeightSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        char scratch_[ 3000 ];
};
