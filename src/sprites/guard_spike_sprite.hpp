#pragma once

#include "counter_t.hpp"
#include "sprite.hpp"
#include "timer_repeat_t.hpp"

class GuardSpikeSprite : public Sprite
{
	public:
		GuardSpikeSprite( int x, int y );
		~GuardSpikeSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
