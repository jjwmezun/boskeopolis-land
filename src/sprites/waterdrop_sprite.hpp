#pragma once

#include "sprite.hpp"

class WaterdropSprite : public Sprite
{
	public:
		WaterdropSprite( int x, int y );
		~WaterdropSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health );
		
	
		void deathAction( Camera& camera, EventSystem& events ) override;
};