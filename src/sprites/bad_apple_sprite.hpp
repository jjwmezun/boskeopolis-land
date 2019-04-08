#pragma once

class Block;

#include "counter_t.hpp"
#include "sprite.hpp"
#include "timer_repeat_t.hpp"

class BadAppleSprite : public Sprite
{
	public:
		BadAppleSprite( int x, int y, Direction::Horizontal dir_x );
		~BadAppleSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		void updateGraphics();

		CounterT<1, 0, 0, true> animation_frame_;
		TimerRepeatT<> animation_timer_;
};
