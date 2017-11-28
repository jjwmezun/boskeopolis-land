#pragma once

class Block;

#include "sprite.hpp"
#include "timers/timer.hpp"

class FallingBoughSprite : public Sprite
{
	public:
		FallingBoughSprite( int x, int y, Direction::Horizontal direction, bool fall = true );
		~FallingBoughSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health ) override;
		void reset() override;

	private:
		bool fall_;
		TimerSimple falling_timer_;
};