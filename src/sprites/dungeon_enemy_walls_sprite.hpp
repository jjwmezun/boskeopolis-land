#pragma once

#include "sprite.hpp"

class DungeonEnemyWallsSprite : public Sprite
{
	public:
		DungeonEnemyWallsSprite( int x, int y );
		~DungeonEnemyWallsSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
};
