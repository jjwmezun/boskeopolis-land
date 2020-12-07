#pragma once

#include "circle_movement.hpp"
#include "sprite.hpp"

class PendulumSprite final : public Sprite
{
	public:
		PendulumSprite( int x, int y );
		~PendulumSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        CircleMovement movement_;
        bool keep_player_;
        int prev_x_;
        bool keep_player_x_;
        char scratch_[3000];
};