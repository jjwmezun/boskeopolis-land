#pragma once

#include "sprite.hpp"

class HookSprite final : public Sprite
{
	public:
		HookSprite( int x, int y );
		~HookSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( Camera& camera, bool priority ) override;
};
