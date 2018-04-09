#pragma once

#include "sprite.hpp"

class SpawnIcicleSprite : public Sprite
{
	public:
		SpawnIcicleSprite( int x, int y );
		~SpawnIcicleSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		int timer_;
		int limit_;
		bool go_;
};