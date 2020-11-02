#pragma once

#include "sprite.hpp"

class DevilSprite final : public Sprite
{
	public:
		DevilSprite( int x, int y );
		~DevilSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        int timer_;
        int timer_target_;
        char scratch_[ 3000 ];
};
