#pragma once

#include "sprite.hpp"

class FallingStarGeneratorSprite final : public Sprite
{
	public:
		FallingStarGeneratorSprite( int x, int y );
		~FallingStarGeneratorSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        void spawn( LevelState& level_state );

        int timer_;
        bool start_;
        int prev_x_[ 3 ];
        char scratch_[ 3000 ];
};
