#pragma once

#include "sprite.hpp"

class AntSprite : public Sprite
{
	public:
		AntSprite( int x, int y );
		~AntSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		Direction::Rotation direction_;
		int slope_timer_;
		int animation_timer_;

		void testSlopeChange( Direction::Rotation next_direction );
};
