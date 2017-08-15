#pragma once

class Sprite;

#include "counter.hpp"
#include "sprite_graphics.hpp"
#include "timer_repeat.hpp"

class PenguinGraphics : public SpriteGraphics
{
	public:
		PenguinGraphics();
		~PenguinGraphics();

		void update( Sprite& sprite );


	private:
		static constexpr int NUM_O_FRAMES = 4;
		Counter animation_frame_;
		TimerRepeat animation_timer_;
};