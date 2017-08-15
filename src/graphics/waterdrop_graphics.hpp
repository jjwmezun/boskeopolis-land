#pragma once

class Sprite;

#include "counter.hpp"
#include "sprite_graphics.hpp"
#include "timer_repeat.hpp"

class WaterdropGraphics : public SpriteGraphics
{
	public:
		WaterdropGraphics();
		~WaterdropGraphics();

		void update( Sprite& sprite );

	private:
		TimerRepeat animation_timer_;
		bool init_;
};