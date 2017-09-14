#pragma once

#include <array>
#include "mezun_sdl2.hpp"
#include <SDL2/SDL_image.h>
#include "timers/timer_repeat.hpp"
#include <vector>

class Palette
{
	public:
	static constexpr int COLOR_LIMIT = 7;
	static constexpr sdl2::SDLColor BLACK = { 0, 0, 0, 255 };
	static std::vector<std::string> palette_names_;
	static std::vector<std::array<sdl2::SDLColor, COLOR_LIMIT>> palettes_;
	static std::array<sdl2::SDLColor, COLOR_LIMIT>* current_palette_;

	std::string type_;
	int bg_;

	Palette( std::string type, int bg );
	bool operator!= ( const Palette& p ) const;
	const Palette& operator= ( const Palette& p );

	const std::string& type() const;
	int bgN() const;
	void applyPalette( SDL_Surface* s ) const;
	const sdl2::SDLColor& color( int n = 1 ) const;
	const sdl2::SDLColor& bg() const;
	bool isNeon() const;
	int testColor( int n ) const;
	void setPalette() const;

	static void loadPalettes();
	static int idFromString( const std::string& type_name );
};