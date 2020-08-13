#pragma once

#include "counter_t.hpp"
#include "sprite.hpp"
#include "timer_repeat_t.hpp"

class ToySoldierSprite final : public Sprite
{
	public:
		ToySoldierSprite( int x, int y );
		~ToySoldierSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

		static constexpr int NUM_O_FRAMES = 4;

	private:
		void updateGraphics();

		CounterT<NUM_O_FRAMES-1, 0, 0, true> animation_frame_;
		TimerRepeatT<> animation_timer_;
};
