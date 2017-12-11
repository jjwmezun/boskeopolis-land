#pragma once

#include "sprite.hpp"

class ShmupBulletSprite : public Sprite
{
	public:
		ShmupBulletSprite( int x, int y, double dy, double dx );
		~ShmupBulletSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
	
	private:
		const double angle_;
};