#pragma once

class Sprite;

#include "sprite_graphics.hpp"

class PlayerCartGraphics : public SpriteGraphics
{
	public:
		PlayerCartGraphics();
		~PlayerCartGraphics();

		void update( Sprite& sprite );
};