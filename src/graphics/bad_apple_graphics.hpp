#pragma once

class Sprite;

#include "counter.hpp"
#include "sprite_graphics.hpp"
#include "timer_repeat.hpp"

class BadAppleGraphics : public SpriteGraphics
{
	public:
		BadAppleGraphics();
		~BadAppleGraphics();

		void update( Sprite& sprite );

	private:
		Counter animation_frame_;
		TimerRepeat animation_timer_;
};