#pragma once

class Block;

#include "sprite.hpp"

class GhostSprite final : public Sprite
{
	public:
		GhostSprite( int x, int y, Direction::Horizontal direction, int speed = 2 );
		~GhostSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		int animation_;
		int tongue_frame_;
		int outline_frame_;
};