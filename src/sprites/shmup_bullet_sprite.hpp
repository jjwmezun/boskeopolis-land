#pragma once

#include "sprite.hpp"

class ShmupBulletSprite final : public Sprite
{
	public:
		ShmupBulletSprite( int x, int y, double dy, double dx );
		~ShmupBulletSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
	
	private:
		const double angle_;
};