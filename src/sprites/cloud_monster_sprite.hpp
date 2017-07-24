#pragma once

class Block;


#include "lightning_sprite.hpp"
#include "sprite.hpp"

class CloudMonsterSprite : public Sprite
{
	public:
		CloudMonsterSprite( int x, int y );
		~CloudMonsterSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health );
		void render( Camera& camera, bool priority ) override;

	private:
		LightningSprite lightning_;
		int lightningPosition() const;
};