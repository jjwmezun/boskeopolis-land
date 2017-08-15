#pragma once

class Sprite;

#include "sprite_graphics.hpp"
#include "maze_chaser_sprite.hpp"

class MazeChaserGraphics : public SpriteGraphics
{
	public:
		MazeChaserGraphics( MazeChaserSprite::Type type );
		~MazeChaserGraphics();

		void update( Sprite& sprite );

	private:
		int typesImgXFrame( MazeChaserSprite::Type type ) const;
};