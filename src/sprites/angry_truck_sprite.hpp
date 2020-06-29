#pragma once

class Block;

#include "sprite.hpp"

class AngryTruckSprite final : public Sprite
{
	public:
		AngryTruckSprite( int x, int y, bool permanent, Direction::Horizontal direction );
		~AngryTruckSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;

	private:
		bool parked_;
};
