#pragma once

class Block;

#include "sprite.hpp"

class HoppingClockSprite final : public Sprite
{
	public:
		HoppingClockSprite( int x, int y );
		~HoppingClockSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
        char scratch_[3000];
};
