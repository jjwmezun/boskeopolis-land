#pragma once

#include "sprite.hpp"

class WaterdropSprite final : public Sprite
{
	public:
		WaterdropSprite( int x, int y );
		~WaterdropSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
		void deathAction( LevelState& level_state ) override;
};
