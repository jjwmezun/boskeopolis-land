#pragma once

class Block;

#include "sprite.hpp"
#include "timer_simple_t.hpp"

class FallingBoughSprite final : public Sprite
{
	public:
		FallingBoughSprite( int x, int y, Direction::Horizontal direction, bool fall = true );
		~FallingBoughSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;

	private:
		bool fall_;
		TimerSimpleT<8, false> falling_timer_;
};
