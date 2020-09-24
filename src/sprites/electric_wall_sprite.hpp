#pragma once

#include "sprite.hpp"

class ElectricWallSprite final : public Sprite
{
	public:
		ElectricWallSprite( int x, int y );
		~ElectricWallSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( Camera& camera, bool priority ) override;
};
