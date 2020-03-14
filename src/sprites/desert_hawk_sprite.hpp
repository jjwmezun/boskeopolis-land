#pragma once

#include "sprite.hpp"

class DesertHawkSprite final : public Sprite
{
	public:
		DesertHawkSprite( int x, int y, Direction::Horizontal direction );
		~DesertHawkSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		int timer_ = 0;
		int frame_ = 0;
};
