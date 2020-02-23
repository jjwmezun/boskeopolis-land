#pragma once

#include "sprite.hpp"

class PedestrianGeneratorSprite : public Sprite
{
	public:
		PedestrianGeneratorSprite( int x, int y );
		~PedestrianGeneratorSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
