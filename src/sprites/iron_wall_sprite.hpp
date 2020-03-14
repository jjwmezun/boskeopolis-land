#pragma once

#include "sprite.hpp"

class IronWallSprite final : public Sprite
{
	public:
		IronWallSprite( int x, int y );
		~IronWallSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
