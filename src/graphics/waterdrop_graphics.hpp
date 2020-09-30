#pragma once

class Sprite;

#include "counter.hpp"
#include "sprite_graphics.hpp"
#include "timer_repeat.hpp"

class WaterdropGraphics final : public SpriteGraphics
{
	public:
		WaterdropGraphics();
		~WaterdropGraphics();

		void update( Sprite& sprite );

	private:
		bool init_;
		TimerRepeat animation_timer_;
};
