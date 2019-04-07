#pragma once

#include "faucet_graphics.hpp"
#include "sprite.hpp"

class FaucetSprite : public Sprite
{
	public:
		FaucetSprite( int x, int y );
		~FaucetSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );
		void render( Camera& camera, bool priority = false ) override;
		int getHits() const;

	private:
		FaucetGraphics gfx_component_;
		int hits_;
		int invincibility_;
		int slide_lock_;
};
