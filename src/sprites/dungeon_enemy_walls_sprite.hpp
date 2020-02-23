#pragma once

#include "sprite.hpp"

class DungeonEnemyWallsSprite : public Sprite
{
	public:
		DungeonEnemyWallsSprite( int x, int y, Direction::Simple direction );
		~DungeonEnemyWallsSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
