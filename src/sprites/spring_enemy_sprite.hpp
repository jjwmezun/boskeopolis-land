#pragma once

#include "sprite.hpp"

class SpringEnemySprite : public Sprite
{
	public:
		SpringEnemySprite( int x, int y );
		~SpringEnemySprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
