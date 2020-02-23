#pragma once

#include "sprite.hpp"
#include "sprite_graphics.hpp"

class SimpleProjectileSprite : public Sprite
{
	public:
		SimpleProjectileSprite( int x, int y, int w, int h, Direction::Horizontal dir, std::unique_ptr<SpriteGraphics>&& gfx );
		~SimpleProjectileSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
};
