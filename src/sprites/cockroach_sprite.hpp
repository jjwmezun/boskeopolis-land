#pragma once

#include "counter_t.hpp"
#include "sprite.hpp"
#include "timer_repeat_t.hpp"

class CockroachSprite final : public Sprite
{
	public:
		CockroachSprite( int x, int y );
		~CockroachSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		int timer_;
        char scratch_[ 3000 ];
};
