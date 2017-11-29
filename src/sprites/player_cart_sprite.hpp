#pragma once

class Block;    
class Sprite;

#include "sprite.hpp"

class PlayerCartSprite : public Sprite
{
	public:
		PlayerCartSprite( int x, int y );
		~PlayerCartSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health ) override;
		void deathAction( Camera& camera, EventSystem& events ) override;

	private:
		static const int JUMP_LIMIT = Unit::BlocksToSubPixels( 32 );
		bool reached_height_ = false;
		void duck();
		void unduck();
};