#pragma once

#include "sprite.hpp"

class CapnClearbeardSprite : public Sprite
{
	public:
		CapnClearbeardSprite( int x, int y );
		~CapnClearbeardSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		int jump_timer_;
		int something1;
		int something2;
		int something3;
		int something4;
		int something5;
		bool move_;
};