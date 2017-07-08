

// Name
//===================================
//
// Palette
//


#ifndef PALETTE_H
#define PALETTE_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

	#include <array>
    #include "mezun_sdl2.h"
    #include <unordered_map>
    #include <SDL2/SDL_image.h>
    #include "timers/timer_repeat.h"


// CLASS
//===================================

    class Palette
	{
		public:
			Palette( std::string type, int bg );
			bool operator!= ( const Palette& p ) const;
			const Palette& operator= ( const Palette& p );

			const std::string& type() const;
			int bgN() const;

            void applyPalette( SDL_Surface* s ) const;
			
            const sdl2::SDLColor& color( unsigned int n = 1 ) const;
            const sdl2::SDLColor& bg() const;
            Uint8 bgR() const;
            Uint8 bgG() const;
            Uint8 bgB() const;
            Uint8 bgA() const;


        private:
			static constexpr int COLOR_LIMIT = 7;
			static std::unordered_map<std::string, std::array<sdl2::SDLColor, COLOR_LIMIT>> palettes_	;
			
			std::string type_;
			int bg_;

            unsigned int testColor( unsigned int n ) const;
			
			void loadPalettes() const;
    };

#endif // PALETTE_H
