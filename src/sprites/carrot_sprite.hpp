#pragma once

#include "sprite.hpp"

class CarrotSprite final : public Sprite
{
	public:
		CarrotSprite( int x, int y );
		~CarrotSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		int timer_;
};