#pragma once

#include "direction.hpp"
#include "sprite.hpp"

class TopDownBulletSprite final : public Sprite
{
	public:
		TopDownBulletSprite( int x, int y, Direction::Simple direction, SpriteType type, int layer );
		~TopDownBulletSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		Direction::Simple direction_;
};
