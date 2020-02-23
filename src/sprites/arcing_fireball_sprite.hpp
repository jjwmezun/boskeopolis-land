#pragma once

#include "sprite.hpp"

class ArcingFireballSprite : public Sprite
{
	public:
		ArcingFireballSprite( int x, int y, int start_speed, int top_speed, int jump_start_speed, int jump_top_speed, Direction::Horizontal direction );
		~ArcingFireballSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
