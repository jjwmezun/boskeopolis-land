#pragma once

class Sprite;

#include "counter.hpp"
#include <map>
#include "sprite_graphics.hpp"
#include "timer_repeat.hpp"
#include "timer_simple.hpp"

class PlayerGraphics : public SpriteGraphics
{
	public:
		PlayerGraphics( std::string&& texture = "sprites/autumn.png" );
		~PlayerGraphics();

		void update( Sprite& sprite );


	private:
		TimerRepeat animation_timer_;
		Counter walk_counter_;
		Counter climb_counter_;
		Counter blink_counter_;
		Counter swim_counter_;
		TimerSimple swim_timer_;
};