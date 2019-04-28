#pragma once

#include "direction.hpp"
#include "sprite.hpp"

class TopDownBulletSprite : public Sprite
{
	public:
		TopDownBulletSprite( int x, int y, Direction::Simple direction, SpriteType type, int layer );
		~TopDownBulletSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		Direction::Simple direction_;
};
