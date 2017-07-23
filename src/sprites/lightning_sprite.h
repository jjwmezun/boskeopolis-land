#pragma once

class Block;
class Input;

#include "sprite.h"

class LightningSprite : public Sprite
{
	public:
		LightningSprite( int x, int y );
		~LightningSprite();
		void customUpdate( const Input& input, Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
		
	private:
		bool on_;
		int counter_;
};