#pragma once

#include "sprite.hpp"

class RisingPlatformSprite final : public Sprite
{
	public:
		RisingPlatformSprite( int x, int y );
		~RisingPlatformSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
