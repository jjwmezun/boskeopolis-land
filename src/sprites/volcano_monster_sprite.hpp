#pragma once

#include "sprite.hpp"

class SpriteSystem;

class VolcanoMonsterSprite : public Sprite
{
	public:
		VolcanoMonsterSprite( int x, int y );
		~VolcanoMonsterSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		int timer_;
		int next_spit_;

		void spawn( SpriteSystem& sprites );
};
