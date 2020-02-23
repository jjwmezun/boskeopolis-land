#pragma once

#include "sprite.hpp"

class OliveSprite : public Sprite
{
	public:
		OliveSprite( int x, int y, Direction::Horizontal start_dir );
		~OliveSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};