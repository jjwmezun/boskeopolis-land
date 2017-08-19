#pragma once

#include "mezun_sdl2.hpp"

class Palette
{
	public:
		static constexpr int NUM_O_NEON_COLORS = 12;

		Palette( int id, int bg );
		Palette( const char* name, int bg );
		bool operator!= ( const Palette& p ) const;
		//const Palette& operator= ( const Palette& p ) = default;

		int id() const;
		int bgN() const;
		bool neon() const;

		void applyPalette( SDL_Surface* s ) const;
		void applyPaletteNeon( SDL_Surface* s, int n ) const;

		const sdl::color& color( int n = 1 ) const;
		const sdl::color& bg() const;
		Uint8 bgR() const;
		Uint8 bgG() const;
		Uint8 bgB() const;
		Uint8 bgA() const;

		static void loadPalettes();


	private:
		static constexpr int NEON_ID = -1;
		static constexpr int PALETTE_LIMIT = 100;
		static constexpr int COLOR_LIMIT = 7;
		static constexpr int NAME_LIMIT = 20;
		static constexpr sdl::color BLACK = { 0, 0, 0, 255 };

		static sdl::color palettes_[ PALETTE_LIMIT ][ COLOR_LIMIT ];
		static char palette_names_[ PALETTE_LIMIT ][ NAME_LIMIT ];
		static int num_o_palettes_;

		int id_;
		int bg_;

		static int stringID( const char* type );
};