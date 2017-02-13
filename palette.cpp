

// Name
//===================================
//
// Palette
//


// DEPENDENCIES
//===================================

    #include "game.h"
    #include "palette.h"
    #include <iostream>


// STATIC PROPERTIES
//===================================

    constexpr sdl2::SDLColor Palette::PALETTE_SETS[][ Palette::COLOR_LIMIT ];

    std::map<const std::string, const Palette::PaletteType> Palette::PaletteConversion
    {
        { "Grayscale", PaletteType::GRAYSCALE },
        { "ClassicGreen", PaletteType::CLASSIC_GREEN },
        { "MidnightBlue", PaletteType::MIDNIGHT_BLUE },
        { "DustyBrown", PaletteType::DUSTY_BROWN },
        { "", PaletteType::SUNNY_YELLOW },
        { "", PaletteType::ICY_BLUE },
        { "", PaletteType::MOUNTAIN_RED },
        { "", PaletteType::MUSTARD_ORANGE },
        { "", PaletteType::POISON_PURPLE },
        { "", PaletteType::SUBTERRANEAN_TEAL },
        { "", PaletteType::BABY_BLUE },
        { "", PaletteType::TRISTE_BLUE },
        { "", PaletteType::PEARL_PINK },
        { "", PaletteType::CHOCO_BROWN },
        { "", PaletteType::UGLY_BROWN },
        { "", PaletteType::SEWER_GREEN },
        { "", PaletteType::MIDNIGHT_RED }
    };


// METHODS
//===================================

    void Palette::applyPalette( SDL_Surface* s ) const
    {
        if ( s != nullptr )
        {
            for ( int i = 0; i < COLOR_LIMIT; ++i )
            {
                // Obviously we should abort if we reach beyond the # o' colors in the surface's array.
                if ( i > s->format->palette->ncolors )
                {
                    return;
                }
                else
                {
                    s->format->palette->colors[i] = colors_[i];
                }
            }
        }
    };

    const sdl2::SDLColor& Palette::getColor( unsigned int n ) const
    {
        if ( n > 0 && n < COLOR_LIMIT )
            return colors_[ n ];
        else
            return colors_[ 1 ];
    };

    const sdl2::SDLColor& Palette::bg() const
    {
        if ( bg_ > 0 )
        {
            if ( bg_ >= COLOR_LIMIT )
            {
                if ( Game::nextFrame( 256, 8 ) )
                    return colors_[ 1 ];
                else
                    return colors_[ 3 ];
            }
            else
            {
                return colors_[ bg_ ];
            }
        }
        else
        {
            return colors_[ 0 ];
        }
    };

    Uint8 Palette::bgR() const { return bg().r; };
    Uint8 Palette::bgG() const { return bg().g; };
    Uint8 Palette::bgB() const { return bg().b; };
    Uint8 Palette::bgA() const { return bg().a; };
