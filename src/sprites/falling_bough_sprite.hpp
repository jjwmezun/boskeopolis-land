#pragma once

class Block;

#include "sprite.hpp"
#include "timer_simple_t.hpp"

class FallingBoughSprite : public Sprite
{
	public:
		FallingBoughSprite( int x, int y, Direction::Horizontal direction, bool fall = true );
		~FallingBoughSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void reset() override;

	private:
		bool fall_;
		TimerSimpleT<8, false> falling_timer_;
};
