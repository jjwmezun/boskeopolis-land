#pragma once

class Block;

#include "sprite.hpp"

class GhostSprite : public Sprite
{
	public:
		GhostSprite( int x, int y );
		~GhostSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		int animation_ = 0;
		int tongue_frame_ = 0;
		int outline_frame_ = 0;
};