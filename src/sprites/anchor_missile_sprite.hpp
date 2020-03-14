#pragma once

#include "direction.hpp"
#include "sprite.hpp"

class AnchorMissileSprite final : public Sprite
{
	public:
		AnchorMissileSprite( int x, int y, Direction::Simple direction );
		~AnchorMissileSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
