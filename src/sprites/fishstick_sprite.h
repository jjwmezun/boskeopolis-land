#pragma once

class Block;
class Input;

#include "sprite.h"

class FishstickSprite : public Sprite
{
	public:
		FishstickSprite( int x, int y );
		~FishstickSprite();
		void customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );

	private:
		TimerSimple move_timer_;
		TimerSimple pause_timer_;
};