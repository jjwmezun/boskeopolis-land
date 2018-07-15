#pragma once

#include "sprite.hpp"

class ArcingFireballSprite : public Sprite
{
	public:
		ArcingFireballSprite( int x, int y, int start_speed, int top_speed, int jump_start_speed, int jump_top_speed, Direction::Horizontal direction );
		~ArcingFireballSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
};
