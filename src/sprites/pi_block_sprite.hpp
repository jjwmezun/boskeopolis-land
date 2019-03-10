#pragma once

#include "sprite.hpp"

class PiBlockSprite : public Sprite
{
	public:
		PiBlockSprite( int x, int y );
		~PiBlockSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void reset() override;

	private:
		int fall_timer_;
		bool hero_still_on_block_;
};
