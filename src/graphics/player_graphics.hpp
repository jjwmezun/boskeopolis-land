#pragma once

class Sprite;
class SpriteGraphics;

#include "counter_t.hpp"
#include "timer_repeat.hpp"
#include "timer_simple.hpp"

class PlayerGraphics
{
	public:
		PlayerGraphics();
		~PlayerGraphics();
		void update( const Sprite& sprite, SpriteGraphics* graphics );

	private:
		TimerRepeat animation_timer_;
		CounterT<3, 0, 0, true> walk_counter_;
		CounterT<1, 0, 0, true> climb_counter_;
		CounterT<11, 0, 0, true> blink_counter_;
		CounterT<2, 0, 0, true> swim_counter_;
		TimerSimple swim_timer_;
};
