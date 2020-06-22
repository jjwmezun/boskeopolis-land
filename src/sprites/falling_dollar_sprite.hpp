#pragma once

#include "sprite.hpp"

class FallingDollarSprite final : public Sprite
{
	public:
		FallingDollarSprite( int x, int y );
		~FallingDollarSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		double center_y_;
		double center_x_;
		double angle_;
		char scratch_[3000];
};
