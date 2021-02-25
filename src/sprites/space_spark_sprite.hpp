#pragma once

#include "sprite.hpp"

class SpaceSparkSprite final : public Sprite
{
	public:
		SpaceSparkSprite( int x, int y, Direction::Horizontal direction );
		~SpaceSparkSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		int left_edge_;
		int right_edge_;
};
