#pragma once

#include "sprite_graphics.hpp"

class SwitchGraphics final : public SpriteGraphics
{
	public:
		SwitchGraphics
		(
			std::unique_ptr<SpriteGraphics> gfx_off,
			std::unique_ptr<SpriteGraphics> gfx_on
		);
		void update( const EventSystem& events ) override;
		void render( const sdl2::SDLRect& bound_box, const Camera* camera = nullptr ) const override;

	private:
		SpriteGraphics* current_gfx_;
		std::unique_ptr<SpriteGraphics> gfx_off_;
		std::unique_ptr<SpriteGraphics> gfx_on_;
};
