#pragma once

#include "bump_under_block_sprite.hpp"
#include "sprite.hpp"

class ColoredSwitchBlockSprite final : public Sprite
{
	public:
		ColoredSwitchBlockSprite( int x, int y, int value );
		~ColoredSwitchBlockSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		BumpUnderBlockSprite bump_under_block_component;
		int value_;
};
