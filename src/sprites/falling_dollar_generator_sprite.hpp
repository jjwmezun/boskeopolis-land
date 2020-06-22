#pragma once

#include "sprite.hpp"

class FallingDollarGeneratorSprite final : public Sprite
{
	public:
		FallingDollarGeneratorSprite( int x, int y );
		~FallingDollarGeneratorSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        int timer_;
};
