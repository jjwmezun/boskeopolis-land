#pragma once

#include "sprite_graphics.hpp"

class SwitchGraphics final : public SpriteGraphics
{
	public:
		SwitchGraphics
		(
			std::vector<std::unique_ptr<SpriteGraphics>>&& gfx
		);
		void update( const EventSystem& events ) override;
		void render( const sdl2::SDLRect& bound_box, const Camera* camera = nullptr ) const override;

	private:
		SpriteGraphics* current_gfx_;
		std::vector<std::unique_ptr<SpriteGraphics>> gfx_;
};
