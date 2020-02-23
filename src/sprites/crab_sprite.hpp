#pragma once

#include "sprite.hpp"

class CrabSprite : public Sprite
{
	public:
		CrabSprite( int x, int y );
		~CrabSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		Direction::Horizontal next_direction_;
		int timer_;
		int animation_timer_;
};
