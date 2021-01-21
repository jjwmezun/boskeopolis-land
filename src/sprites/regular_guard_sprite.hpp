#pragma once

#include "sprite.hpp"

class RegularGuardSprite final : public Sprite
{
	public:
		RegularGuardSprite( int x, int y, Direction::Horizontal direction );
		~RegularGuardSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
