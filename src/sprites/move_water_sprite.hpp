#pragma once

#include "sprite.hpp"

class MoveWaterSprite final : public Sprite
{
	public:
		MoveWaterSprite( int x, int y );
		~MoveWaterSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		Direction::Clockwise spin_direction_;
		void readjustGraphics();
};
