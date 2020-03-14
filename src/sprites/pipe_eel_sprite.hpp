#pragma once

class Block;

#include "sprite.hpp"

class PipeEelSprite final : public Sprite
{
	public:
		PipeEelSprite( int x, int y, Direction::Vertical dir_y );
		~PipeEelSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
};