#pragma once

class Block;

#include "sprite.hpp"

class BirdSprite final : public Sprite
{
	public:
		BirdSprite( int x, int y, Direction::Horizontal dir );
		~BirdSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;

	private:
		int wait_limit_counter_;
		int move_delay_counter_;
		int propeller_animation_timer_;
		bool flip_x_;
};