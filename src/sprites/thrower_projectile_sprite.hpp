#pragma once

#include "sprite.hpp"

class ThrowerProjectileSprite final : public Sprite
{
	public:
		ThrowerProjectileSprite( int x, int y, Direction::Horizontal direction );
		~ThrowerProjectileSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
