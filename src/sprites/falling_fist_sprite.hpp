#pragma once

class Block;

#include "sprite.hpp"

class FallingFistSprite : public Sprite
{
	public:
		FallingFistSprite( int x, int y );
		~FallingFistSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health ) override;
		void render( Camera& camera, bool priority = false ) override;

	private:
		void drawCrane( const Camera& camera );
};