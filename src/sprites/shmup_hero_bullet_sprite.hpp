#pragma once

#include "sprite.hpp"

class ShmupHeroBulletSprite : public Sprite
{
	public:
		ShmupHeroBulletSprite( int x, int y, Direction::Simple direction, SpriteType type );
		~ShmupHeroBulletSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		Direction::Simple direction_;
};
