#pragma once

class AnimatedGraphics;

class OWCamera;

class OWLevel
{
	public:
		OWLevel( int type, int x, int y, const AnimatedGraphics& gfx );
		void render( const OWCamera& camera ) const;
		sdl::rect coords() const;
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
	
		sdl::rect src() const;
		sdl::rect dest( const OWCamera& camera ) const;
};