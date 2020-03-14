#pragma once

class Block;

#include "sprite.hpp"

class GhostSprite final : public Sprite
{
	public:
		GhostSprite( int x, int y );
		~GhostSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		int animation_ = 0;
		int tongue_frame_ = 0;
		int outline_frame_ = 0;
};