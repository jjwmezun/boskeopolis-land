

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

    #include "bosk_sdl2.h"
    #include <map>
    #include <SDL2/SDL_image.h>
    #include "timers/timer_repeat.h"


// CLASS
//===================================

    class Palette
    {
        public:
            static const Uint8 DEFAULT_VALUE = 255;

            // Don't apply specific #s.
            enum class PaletteType
            {
                GRAYSCALE,
                CLASSIC_GREEN,
                MIDNIGHT_BLUE,
                DUSTY_BROWN,
                SUNNY_YELLOW,
                ICY_BLUE,
                MOUNTAIN_RED,
                MUSTARD_ORANGE,
                POISON_PURPLE,
                SUBTERRANEAN_TEAL,
                BABY_BLUE,
                TRISTE_BLUE,
                PEARL_PINK,
                CHOCO_BROWN,
                UGLY_BROWN,
                SEWER_GREEN,
                MIDNIGHT_RED,
                NEON,
                SHADOW,
                __NULL // Must be last.
            };

            static std::map<const std::string, const PaletteType> PaletteConversion;

            class PaletteSet
            {
                private:
                    PaletteType type_;
                    int bg_;

                public:
                    constexpr PaletteSet( PaletteType type, int bg )
                    :
                        type_ ( type ),
                        bg_ ( bg )
                    {};

                    constexpr bool operator!= ( const PaletteSet& p ) const
                    {
                        return ( type_ != p.type() || bg_ != p.bg() );
                    };

                    constexpr PaletteType type() const { return type_; };
                    constexpr int bg() const { return bg_; };
            };

            //Palette( PaletteType p = PaletteType::GRAYSCALE, unsigned int bg = PaletteType::GRAYSCALE );
            //Palette( PaletteSet p );

            constexpr Palette( PaletteType p = PaletteType::GRAYSCALE, unsigned int bg = 1 )
            :
                colors_ ( PALETTE_SETS[ testPal( p ) ] ),
                bg_ ( bg )
            {};

            constexpr Palette( PaletteSet p )
            :
                colors_ ( PALETTE_SETS[ testPal( p.type() ) ] ),
                bg_ ( p.bg() )
            {};

            void applyPalette( SDL_Surface* s ) const;
            const sdl2::SDLColor& getColor( unsigned int n = 1 ) const;
            const sdl2::SDLColor& bg() const;
            Uint8 bgR() const;
            Uint8 bgG() const;
            Uint8 bgB() const;
            Uint8 bgA() const;

        private:
            static const int COLOR_LIMIT = 7;
            static const int PALETTE_LIMIT = (int)PaletteType::__NULL;
            static constexpr sdl2::SDLColor PALETTE_SETS[ PALETTE_LIMIT ][ COLOR_LIMIT ] =
            {
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 161, 161, 161, 255 },
                    { 121, 121, 121, 255 },
                    {  81,  81,  81, 255 },
                    {  40,  40,  40, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 190, 241,  51, 255 },
                    { 139, 172,  15, 255 },
                    {  48,  98,  48, 255 },
                    {  15,  56,  15, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 234, 195, 255, 255 },
                    { 151, 120, 198, 255 },
                    { 105,  80, 130, 255 },
                    {  51,  36,  42, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 234, 230, 153, 255 },
                    { 183, 162, 102, 255 },
                    { 141, 105,  61, 255 },
                    {  94,  43,  14, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 227, 213, 136, 255 },
                    { 160, 146,  95, 255 },
                    { 105,  90,  61, 255 },
                    {  49,  34,  28, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 175, 188, 217, 255 },
                    { 133, 139, 159, 255 },
                    {  98,  94, 107, 255 },
                    {  51,  39,  43, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 255, 183, 185, 255 },
                    { 222, 123, 124, 255 },
                    { 171,  73,  72, 255 },
                    {  28,  26,  27, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 248, 192, 136, 255 },
                    { 248, 128,  79, 255 },
                    { 170,  79,  43, 255 },
                    {  47,  29,  25, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 203, 165, 236, 255 },
                    { 160,  71, 236, 255 },
                    {  81,  42,  94, 255 },
                    {  43,  25,  32, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 174, 255, 255, 255 },
                    {  77, 151, 117, 255 },
                    {  22,  63,  51, 255 },
                    {   0,  10,   6, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 163, 221, 254, 255 },
                    {  83, 168, 217, 255 },
                    {  27, 103, 147, 255 },
                    {   0,  28,  61, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 165, 168, 230, 255 },
                    { 116, 120, 186, 255 },
                    {  69,  72, 128, 255 },
                    {  24,  25,  57, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 255, 191, 216, 255 },
                    { 229, 136, 172, 255 },
                    { 194,  92, 131, 255 },
                    {  94,  33,  57, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 163, 150, 144, 255 },
                    { 108,  94,  88, 255 },
                    {  63,  50,  44, 255 },
                    {  24,  15,  11, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 230, 160, 123, 255 },
                    { 199, 105,  54, 255 },
                    { 128,  61,  23, 255 },
                    {  48,   8,   0, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 255, 255, 255, 255 },
                    { 160, 178, 132, 255 },
                    { 113, 134,  83, 255 },
                    {  67,  86,  41, 255 },
                    {  21,  33,   3, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    { 255, 255, 255, 0   },
                    { 254, 100, 126, 255 },
                    { 204,   2,  35, 255 },
                    { 139,   1,  24, 255 },
                    {  75,   1,  13, 255 },
                    {  19,   0,   2, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    {   0,   0,   0,   0 },
                    {   0,   0,   0, 255 },
                    {   0,   0,   0, 255 },
                    {   0,   0,   0, 255 },
                    {   0,   0,   0, 255 },
                    { 255,   0,   0, 255 },
                    {   0,   0,   0, 255 }
                },
                {
                    {   0,   0,   0,   0 },
                    { 255, 255, 255, 255 },
                    {   0,   0,   0, 255 },
                    {   0,   0,   0, 255 },
                    {   0,   0,   0, 255 },
                    {   0,   0,   0, 255 },
                    {  40,  40,  40, 255 }
                }
            };

            sdl2::SDLColor colors_[ COLOR_LIMIT ];
            const unsigned int bg_ = 1;

            //unsigned int testColor( unsigned int n ) const;
            //unsigned int testPal( Palette::PaletteType p ) const;

            constexpr unsigned int testPal( Palette::PaletteType p ) const
            {
                return ( (int)p >= 0 && (int)p < PALETTE_LIMIT ) ? (int)p : (unsigned int)PaletteType::GRAYSCALE;
            };

            constexpr unsigned int testColor( unsigned int n ) const
            {
                return ( n > 0 && n < COLOR_LIMIT ) ? n : 1;
            };
    };

#endif // PALETTE_H


