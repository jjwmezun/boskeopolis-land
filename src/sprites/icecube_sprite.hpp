#pragma once

class Block;

#include "sprite.hpp"

class IceCubeSprite final : public Sprite
{
	public:
		IceCubeSprite( int x, int y, Direction::Horizontal direction = Direction::Horizontal::RIGHT, bool start_moving = false );
		~IceCubeSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
		void reset();

	private:
		bool start_moving_;
		bool start_moving_orig_;
		bool changed_;
};