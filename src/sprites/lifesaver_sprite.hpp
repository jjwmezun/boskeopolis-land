#pragma once

class Block;

#include "sprite.hpp"

class LifesaverSprite final : public Sprite
{
	public:
		LifesaverSprite( int x, int y, bool crate = false );
		~LifesaverSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
		void reset();

	private:
		Sprite* spout_;
		bool can_push_down_;
};
