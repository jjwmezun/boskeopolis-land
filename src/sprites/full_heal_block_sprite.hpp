#pragma once

#include "bump_under_block_sprite.hpp"
#include "sprite.hpp"

class FullHealBlockSprite : public Sprite
{
	public:
		FullHealBlockSprite( int x, int y );
		~FullHealBlockSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		BumpUnderBlockSprite bump_under_block_component;
};
