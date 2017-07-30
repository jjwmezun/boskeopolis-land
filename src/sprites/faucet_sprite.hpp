#pragma once

#include "sprite.hpp"

class FaucetSprite : public Sprite
{
	public:
		FaucetSprite( int x, int y );
		~FaucetSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health );
		
	private:
		int hits_;
		int invincibility_;
		bool slide_lock_;
};