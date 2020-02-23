#pragma once

#include "sprite.hpp"

class ChangeCameraAutoscrollSprite : public Sprite
{
	public:
		ChangeCameraAutoscrollSprite( int x, int y, Direction::Simple direction );
		~ChangeCameraAutoscrollSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		const Direction::Simple direction_;
};
