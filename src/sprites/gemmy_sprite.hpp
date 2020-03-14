#pragma once

class Block;

#include "sprite.hpp"

class GemmySprite final : public Sprite
{
	public:
		GemmySprite( int x, int y );
		~GemmySprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		int gfx_frame_;
		int gfx_timer_;
};