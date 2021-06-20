#pragma once

class Block;

#include "sprite.hpp"

class TrafficCarSprite final : public Sprite
{
	public:
		TrafficCarSprite( int x, int y, Direction::Horizontal direction, int speed );
		~TrafficCarSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
        char scratch_[3000];
};
