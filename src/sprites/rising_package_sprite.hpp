#pragma once

#include "sprite.hpp"

class RisingPackageSprite : public Sprite
{
	public:
		RisingPackageSprite( int x, int y );
		~RisingPackageSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		bool rise_;
};
