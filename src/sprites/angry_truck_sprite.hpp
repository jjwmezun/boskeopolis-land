#pragma once

class Block;

#include "sprite.hpp"

class AngryTruckSprite : public Sprite
{
	public:
		AngryTruckSprite( int x, int y, bool permanent );
		~AngryTruckSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;

	private:
		bool parked_;
};
