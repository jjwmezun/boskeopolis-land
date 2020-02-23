#pragma once

#include "sprite.hpp"

class PlayerPlaneSprite : public Sprite
{
	public:
		PlayerPlaneSprite( int x, int y );
		~PlayerPlaneSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		double angle_;
		double speed_;
};
