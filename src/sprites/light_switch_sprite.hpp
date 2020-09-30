#pragma once

#include "bump_under_block_sprite.hpp"
#include "sprite.hpp"

class LightSwitchSprite final : public Sprite
{
	public:
		LightSwitchSprite( int x, int y );
		~LightSwitchSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( Camera& camera ) override;

    private:
		BumpUnderBlockSprite bump_under_block_component;
};
