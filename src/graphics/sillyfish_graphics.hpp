#pragma once

class Sprite;

#include "counter.hpp"
#include "sprite_graphics.hpp"
#include "timer_repeat.hpp"

class SillyfishGraphics : public SpriteGraphics
{
	public:
		SillyfishGraphics();
		~SillyfishGraphics();

		void update( Sprite& sprite );


	private:
		static constexpr int NUM_O_FRAMES = 2;
		Counter animation_frame_;
		TimerRepeat animation_timer_;
};