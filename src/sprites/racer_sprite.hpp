#pragma once

class Block;

#include "player_sprite.hpp"

class RacerSprite : public PlayerSprite
{
	public:
		static const bool DEBUG = false;
		RacerSprite( int x, int y );
		~RacerSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );
};