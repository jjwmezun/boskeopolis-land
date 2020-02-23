#pragma once

#include "sprite.hpp"

class OliveSpawnerSprite : public Sprite
{
	public:
		OliveSpawnerSprite( int x, int y, Direction::Horizontal start_dir );
		~OliveSpawnerSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};