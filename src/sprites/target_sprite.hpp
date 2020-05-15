#pragma once

#include "sprite.hpp"

class TargetSprite final : public Sprite
{
	public:
		TargetSprite( int x, int y );
		~TargetSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        char scratch_[1000];
};
