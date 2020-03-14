#pragma once

#include "sprite.hpp"

class SpiderSprite final : public Sprite
{
	public:
		SpiderSprite( int x, int y );
		~SpiderSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		Direction::Simple direction_;
		void debugCollision();
};
