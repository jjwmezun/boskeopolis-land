#pragma once

class AnimatedGraphics;
class Graphics;
class OWCamera;

class OWLevel
{
	public:
		OWLevel( int type, int x, int y, const AnimatedGraphics& gfx );
		void render( Graphics& graphics, const OWCamera& camera ) const;
		sdl2::SDLRect coords() const;
		int lv() const;
		int x() const;
		int y() const;

	private:
		static constexpr int SIZE = 16;
		static constexpr int TEXTURE_WIDTH = 16;

		const int type_;
		const int x_;
		const int y_;
		const AnimatedGraphics* gfx_;
	
		sdl2::SDLRect src() const;
		sdl2::SDLRect dest( const OWCamera& camera ) const;
};