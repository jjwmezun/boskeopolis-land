#pragma once

#include "sprite.hpp"

class FrozenMeatSprite final : public Sprite
{
	public:
		FrozenMeatSprite( int x, int y );
		~FrozenMeatSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( Camera& camera, bool priority ) override;

	private:
		sdl2::SDLRect top_hit_box_;
		sdl2::SDLRect image_box_;
		char scratch_[ 3000 ];

		void updateOtherHitBoxes();
};
