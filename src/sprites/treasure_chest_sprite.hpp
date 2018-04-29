#pragma once

#include "sprite.hpp"

class TreasureChestSprite : public Sprite
{
	public:
		TreasureChestSprite( int x, int y );
		~TreasureChestSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void render( Camera& camera, bool priority = false ) override;

	private:
		const sdl2::SDLRect BOTTOM;
};