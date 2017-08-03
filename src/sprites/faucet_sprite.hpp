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
		static constexpr int NUM_O_HITS = 3;
		static constexpr int INVINCIBILITY_FLICKER_SPEED = 4;
		static constexpr int INVINCIBILITY_TIME = 45;
		static constexpr int SLIDE_LOCK_TIME = 5;

		int hits_;
		int invincibility_;
		int slide_lock_;
};