#pragma once

class Block;

#include "sprite.hpp"

class BirdSprite : public Sprite
{
	public:
		BirdSprite( int x, int y, Direction::Horizontal dir );
		~BirdSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health ) override;
		void reset() override;

	private:
		int wait_limit_counter_;
		int move_delay_counter_;
};