#pragma once

class Health;

#include "mezun_sdl2.hpp"
#include <string>

class InventoryLevelHealth final
{
	public:
		InventoryLevelHealth( int x, int y, int max, int bg_color );
		bool update( const Health& health );
		void render();
		int width() const;

	private:
		int hearts_shown_;
		int bg_color_;
		std::string image_;
		sdl2::SDLRect empty_heart_src_;
		sdl2::SDLRect empty_heart_dest_;
		sdl2::SDLRect full_heart_src_;
		sdl2::SDLRect full_heart_dest_;
};