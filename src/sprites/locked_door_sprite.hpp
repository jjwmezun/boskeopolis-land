#pragma once

class Block;

#include "sprite.hpp"

class LockedDoorSprite : public Sprite
{
	public:
		LockedDoorSprite( int x, int y, int map_id );
		~LockedDoorSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void render( Camera& camera, bool priority ) override;
		
	private:
		bool has_key_;
		bool opening_;
};