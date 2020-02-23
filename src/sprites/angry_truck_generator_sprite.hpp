#pragma once

#include "sprite.hpp"

class AngryTruckGeneratorSprite : public Sprite
{
	public:
		AngryTruckGeneratorSprite( int x, int y );
		~AngryTruckGeneratorSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		int timer_;
};
