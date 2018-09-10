#pragma once

#include "sprite_graphics.hpp"

class SwitchGraphics : public SpriteGraphics
{
	public:
		SwitchGraphics
		(
			std::unique_ptr<SpriteGraphics> gfx_off,
			std::unique_ptr<SpriteGraphics> gfx_on
		);
		void update( const EventSystem& events ) override;
		void render( const sdl2::SDLRect& bound_box, const Camera* camera = nullptr, bool priority = false, SDL_Texture* texture = nullptr ) const override;

	private:
		std::unique_ptr<SpriteGraphics> gfx_off_;
		std::unique_ptr<SpriteGraphics> gfx_on_;
		SpriteGraphics* current_gfx_;
};
