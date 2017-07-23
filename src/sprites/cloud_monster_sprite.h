#pragma once

class Block;
class Input;

#include "lightning_sprite.h"
#include "sprite.h"

class CloudMonsterSprite : public Sprite
{
	public:
		CloudMonsterSprite( int x, int y );
		~CloudMonsterSprite();
		void customUpdate( const Input& input, Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
		void render( Camera& camera, bool priority ) override;

	private:
		LightningSprite lightning_;
		int lightningPosition() const;
};