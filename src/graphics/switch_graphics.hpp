#pragma once

class Camera;
class Sprite;

#include "render.hpp"

class SwitchGraphics : public SpriteGraphics
{
	public:
		SwitchGraphics
		(
			std::unique_ptr<SpriteGraphics> gfx_on,
			std::unique_ptr<SpriteGraphics> gfx_off
		);

	private:
		std::unique_ptr<SpriteGraphics> gfx_on_;
		std::unique_ptr<SpriteGraphics> gfx_off_;
};