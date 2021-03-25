#pragma once

#include "sprite.hpp"

class HelicopterFriedmanSprite final : public Sprite
{
	public:
		HelicopterFriedmanSprite( int x, int y );
		~HelicopterFriedmanSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
		int timer_;
        char scratch_[3000];
};
