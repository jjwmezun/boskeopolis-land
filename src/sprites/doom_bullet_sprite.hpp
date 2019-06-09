#pragma once

#include "sprite.hpp"

class DoomBulletSprite : public Sprite
{
	public:
		DoomBulletSprite( int x, int y, double dx, double dy );
		~DoomBulletSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;

	private:
		double dx_;
		double dy_;
};
