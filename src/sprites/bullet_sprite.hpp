#pragma once

#include "sprite.hpp"

class BulletSprite final : public Sprite
{
	public:
		BulletSprite( int x, int y, Direction::Simple dir, bool heros );
		~BulletSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
};