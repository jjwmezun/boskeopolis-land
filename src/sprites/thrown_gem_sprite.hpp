#pragma once

#include "sprite.hpp"

class BombSprite final : public Sprite
{
	public:
		BombSprite( int x, int y );
		~BombSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		bool detonated_;
		char scratch_[3000];
};
