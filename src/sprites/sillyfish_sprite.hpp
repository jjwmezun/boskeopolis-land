#pragma once

#include "counter_t.hpp"
#include "sprite.hpp"
#include "timer_repeat_t.hpp"

class SillyfishSprite : public Sprite
{
	public:
		SillyfishSprite( int x, int y );
		~SillyfishSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		void updateGraphics();
		static constexpr int NUM_O_FRAMES = 2;
		CounterT<NUM_O_FRAMES-1, 0, 0, true> animation_frame_;
		TimerRepeatT<> animation_timer_;
};
