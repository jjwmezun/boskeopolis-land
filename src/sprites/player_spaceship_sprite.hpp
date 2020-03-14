#pragma once

class Block;

#include "sprite.hpp"

class PlayerSpaceshipSprite final : public Sprite
{
	public:
		PlayerSpaceshipSprite( int x, int y );
		~PlayerSpaceshipSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		Direction::Simple direction_;
		Direction::Simple camera_direction_;
};
