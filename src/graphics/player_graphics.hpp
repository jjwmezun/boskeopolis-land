#pragma once

class EventSystem;
class Sprite;
class SpriteGraphics;

#include "counter_t.hpp"
#include "timer_repeat.hpp"
#include "timer_simple_t.hpp"

class PlayerGraphics
{
	public:
		PlayerGraphics();
		~PlayerGraphics();
		void update( const Sprite& sprite, SpriteGraphics* graphics, const EventSystem* events = nullptr );

	private:
		TimerRepeat animation_timer_;
		CounterT<3, 0, 0, true> walk_counter_;
		CounterT<1, 0, 0, true> climb_counter_;
		CounterT<11, 0, 0, true> blink_counter_;
		CounterT<2, 0, 0, true> swim_counter_;
		TimerSimpleT<0, false> swim_timer_;
};
