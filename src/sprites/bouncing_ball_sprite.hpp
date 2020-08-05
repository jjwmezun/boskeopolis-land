#pragma once

#include "sprite.hpp"

class BouncingBallSprite final : public Sprite
{
	public:
		BouncingBallSprite( int x, int y );
		~BouncingBallSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
    private:
        char scratch_[ 3000 ];
};
