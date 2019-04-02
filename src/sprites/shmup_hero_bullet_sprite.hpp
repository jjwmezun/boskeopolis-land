#pragma once

#include "sprite.hpp"

class ShmupHeroBulletSprite : public Sprite
{
	public:
		ShmupHeroBulletSprite( int x, int y, Direction::Simple direction, SpriteType type );
		~ShmupHeroBulletSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		Direction::Simple direction_;
};
