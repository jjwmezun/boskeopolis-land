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
        void spawn( LevelState& level_state ) const;

        int timer_;
        bool start_;
        char scratch_[ 3000 ];
};
