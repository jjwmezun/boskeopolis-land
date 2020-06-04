#pragma once

#include "sprite.hpp"

class HorizontalSparkSprite final : public Sprite
{
	public:
		HorizontalSparkSprite( int x, int y );
		~HorizontalSparkSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
