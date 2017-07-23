#pragma once

class Block;
class Input;

#include "sprite.h"
#include "timers/timer.h"

class FallingBoughSprite : public Sprite
{
	public:
		FallingBoughSprite( int x, int y, Direction::Horizontal direction, bool fall = true );
		~FallingBoughSprite();
		void customUpdate( const Input& input, Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
		void reset();

	private:
		bool fall_;
		TimerSimple falling_timer_;
};