#pragma once

#include "sprite.hpp"

class SwampMonsterSprite final : public Sprite
{
	public:
		SwampMonsterSprite( int x, int y );
		~SwampMonsterSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
