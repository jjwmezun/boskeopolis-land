#pragma once

#include "sprite.hpp"

class IcicleSprite final : public Sprite
{
	public:
		IcicleSprite( int x, int y );
		~IcicleSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};