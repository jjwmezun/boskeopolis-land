#pragma once

#include "sprite.hpp"

class DevilFireSprite final : public Sprite
{
	public:
		DevilFireSprite( int x, int y, double dy, double dx, double start_speed );
		~DevilFireSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
	
	private:
		const double angle_;
        double speed_;
};