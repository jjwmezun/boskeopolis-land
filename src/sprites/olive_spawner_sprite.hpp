#pragma once

#include "sprite.hpp"

class OliveSpawnerSprite : public Sprite
{
	public:
		OliveSpawnerSprite( int x, int y, Direction::Horizontal start_dir );
		~OliveSpawnerSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
};