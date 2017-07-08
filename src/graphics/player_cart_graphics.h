#ifndef PLAYER_CART_GRAPHICS_H
#define PLAYER_CART_GRAPHICS_H

class Sprite;

#include "sprite_graphics.h"

class PlayerCartGraphics : public SpriteGraphics
{
	public:
		PlayerCartGraphics();
		~PlayerCartGraphics();

		void update( Sprite& sprite );
};


#endif // PLAYER_CART_GRAPHICS_H
