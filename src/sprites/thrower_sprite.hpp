#pragma once

#include "sprite.hpp"

class ThrowerSprite final : public Sprite
{
	public:
		ThrowerSprite( int x, int y, Direction::Horizontal direction );
		~ThrowerSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
		int throw_timer_;
		int target_throw_time_;
        char scratch_[ 3000 ];
};
