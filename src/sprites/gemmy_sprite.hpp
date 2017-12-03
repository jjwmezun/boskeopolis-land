#pragma once

class Block;

#include "sprite.hpp"

class GemmySprite : public Sprite
{
	public:
		GemmySprite( int x, int y );
		~GemmySprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		int gfx_frame_;
		int gfx_timer_;
};