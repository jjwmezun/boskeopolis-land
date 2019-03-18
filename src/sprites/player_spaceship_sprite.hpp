#pragma once

class Block;

#include "sprite.hpp"

class PlayerSpaceshipSprite : public Sprite
{
	public:
		PlayerSpaceshipSprite( int x, int y );
		~PlayerSpaceshipSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		Direction::Simple direction_;
		Direction::Simple camera_direction_;
};
