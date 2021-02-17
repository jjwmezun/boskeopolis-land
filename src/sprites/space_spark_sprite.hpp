#pragma once

#include "sprite.hpp"

class SpaceSparkSprite final : public Sprite
{
	public:
		SpaceSparkSprite( int x, int y );
		~SpaceSparkSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
