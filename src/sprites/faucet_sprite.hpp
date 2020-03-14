#pragma once

#include "faucet_graphics.hpp"
#include "sprite.hpp"

class FaucetSprite final : public Sprite
{
	public:
		FaucetSprite( int x, int y );
		~FaucetSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
		void render( Camera& camera, bool priority = false ) override;
		int getHits() const;

	private:
		FaucetGraphics gfx_component_;
		int hits_;
		int invincibility_;
		int slide_lock_;
};
