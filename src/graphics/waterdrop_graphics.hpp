#pragma once

class Sprite;

#include "counter.hpp"
#include "sprite_graphics.hpp"

class WaterdropGraphics : public SpriteGraphics
{
	public:
		WaterdropGraphics();
		~WaterdropGraphics();

		void update( Sprite& sprite );

	private:
		bool init_;
};