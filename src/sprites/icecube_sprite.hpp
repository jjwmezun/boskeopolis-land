#pragma once

class Block;

#include "sprite.hpp"

class IceCubeSprite : public Sprite
{
	public:
		IceCubeSprite( int x, int y, Direction::Horizontal direction = Direction::Horizontal::RIGHT, bool start_moving = false );
		~IceCubeSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );
		void reset();

	private:
		bool start_moving_;
		bool start_moving_orig_;
		bool changed_;
};