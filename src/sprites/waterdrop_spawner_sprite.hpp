#pragma once

#include "sprite.hpp"

class WaterdropSpawnerSprite : public Sprite
{
	public:
		WaterdropSpawnerSprite( int x, int y );
		~WaterdropSpawnerSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );
		
	private:
		int timer_;
};