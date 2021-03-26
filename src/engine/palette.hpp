#pragma once

#include <array>
#include "mezun_sdl2.hpp"
#include <unordered_map>
#include <SDL2/SDL_image.h>
#include "timers/timer_repeat.hpp"

class GameState;

class PaletteChanger
{
	public:
		PaletteChanger();
		void setCurrentPalette( GameState* state, int bg = 1 );
		void setNextPalette( GameState* state, int bg = 1 );
		void setPreviousPalette( GameState* state, int bg = 1 );
	private:
		int number_;
};

class Palette final
{
	public:
		static constexpr int COLOR_LIMIT = 7;
		Palette( std::string type, int bg );
		bool operator!= ( const Palette& p ) const;
		const Palette& operator= ( const Palette& p );
		const std::string& type() const;
		int bgN() const;
		void applyPalette( SDL_Surface* s ) const;
		sdl2::SDLColor color( unsigned int n = 1 ) const;
		sdl2::SDLColor bg() const;
		Uint8 bgR() const;
		Uint8 bgG() const;
		Uint8 bgB() const;
		Uint8 bgA() const;
		static void init();

		std::string type_;
		int bg_;

	private:
		unsigned int testColor( unsigned int n ) const;
};