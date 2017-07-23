#pragma once

class Sprite;

#include "sprite_graphics.hpp"

class RopeGraphics : public SpriteGraphics
{
	public:
		RopeGraphics();
		~RopeGraphics();

		void update( Sprite& sprite );
		void render( const sdl2::SDLRect& bound_box, const Camera* camera = nullptr, bool priority = false, Uint8 alpha = 255 ) const override;

	private:
		static constexpr std::pair<int, int> frame_body_ = { 112, 16 };
		static constexpr std::pair<int, int> frame_end_ = { 120, 16 };
};