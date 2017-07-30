#pragma once

class Sprite;

#include "counter.hpp"
#include "sprite_graphics.hpp"

class BadAppleGraphics : public SpriteGraphics
{
	public:
		BadAppleGraphics();
		~BadAppleGraphics();

		void update( Sprite& sprite );

	private:
		Counter animation_frame_;
};