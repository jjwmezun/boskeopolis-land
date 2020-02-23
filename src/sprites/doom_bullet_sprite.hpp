#pragma once

#include "sprite.hpp"

class DoomBulletSprite : public Sprite
{
	public:
		DoomBulletSprite( int x, int y, double dx, double dy );
		~DoomBulletSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;

	private:
		double dx_;
		double dy_;
};
