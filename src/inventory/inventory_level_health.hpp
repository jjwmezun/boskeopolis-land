#pragma once

class Health;

#include "mezun_sdl2.hpp"
#include <string>

class InventoryLevelHealth
{
	public:
		InventoryLevelHealth( int y );
		bool update( const Health& health );
		void render();

	private:
		int hearts_shown_;
		std::string image_;
		sdl2::SDLRect empty_heart_src_;
		sdl2::SDLRect empty_heart_dest_;
		sdl2::SDLRect full_heart_src_;
		sdl2::SDLRect full_heart_dest_;
};