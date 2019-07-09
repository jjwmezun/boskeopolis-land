#pragma once

#include "sprite.hpp"
#include "sprite_graphics.hpp"

class SimpleProjectileSprite : public Sprite
{
	public:
		SimpleProjectileSprite( int x, int y, int w, int h, Direction::Horizontal dir, std::unique_ptr<SpriteGraphics>&& gfx );
		~SimpleProjectileSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );
};
