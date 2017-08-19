#include <cassert>
#include <fstream>
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "palette.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <SDL2/SDL_image.h>

constexpr sdl::color Palette::BLACK;
sdl::color Palette::palettes_[ PALETTE_LIMIT ][ COLOR_LIMIT ];
char Palette::palette_names_[ PALETTE_LIMIT ][ NAME_LIMIT ] = { '\0' };
int Palette::num_o_palettes_ = 0;

Palette::Palette( int id, int bg )
:
	id_ ( id ),
	bg_ ( bg )
{};

Palette::Palette( const char* name, int bg )
{
	id_ = stringID( name );
	bg_ = bg;
};

bool Palette::operator!= ( const Palette& p ) const
{
	return ( id_ != p.id() || bg_ != p.bgN() );
};

int Palette::id() const { return id_; };
int Palette::bgN() const { return bg_; };
bool Palette::neon() const { return id_ == NEON_ID; };

void Palette::applyPalette( SDL_Surface* s ) const
{
	for ( int i = 0; i < COLOR_LIMIT && i < s->format->palette->ncolors; ++i )
	{
		s->format->palette->colors[ i ] = color( i );
	}
};

void Palette::applyPaletteNeon( SDL_Surface* s, int n ) const
{
	for ( int i = 0; i < COLOR_LIMIT && i < s->format->palette->ncolors; ++i )
	{
		switch ( i )
		{
			case 0:
				s->format->palette->colors[ i ].a = 0;
			break;

			case 5: //fallthrough
			case 6:
				switch ( n )
				{
					case ( 0 ):
						s->format->palette->colors[ i ] = { 255, 0, 0, 255 };
					break;
					case ( 1 ):
						s->format->palette->colors[ i ] = { 255, 128, 0, 255 };
					break;
					case ( 2 ):
						s->format->palette->colors[ i ] = { 255, 255, 0, 255 };
					break;
					case ( 3 ):
						s->format->palette->colors[ i ] = { 128, 255, 0, 255 };
					break;
					case ( 4 ):
						s->format->palette->colors[ i ] = { 0, 255, 0, 255 };
					break;
					case ( 5 ):
						s->format->palette->colors[ i ] = { 0, 255, 128, 255 };
					break;
					case ( 6 ):
						s->format->palette->colors[ i ] = { 0, 255, 255, 255 };
					break;
					case ( 7 ):
						s->format->palette->colors[ i ] = { 0, 128, 255, 255 };
					break;
					case ( 8 ):
						s->format->palette->colors[ i ] = { 0, 0, 255, 255 };
					break;
					case ( 9 ):
						s->format->palette->colors[ i ] = { 128, 0, 255, 255 };
					break;
					case ( 10 ):
						s->format->palette->colors[ i ] = { 255, 0, 255, 255 };
					break;
					case ( 11 ):
						s->format->palette->colors[ i ] = { 255, 0, 128, 255 };
					break;

					default:
						throw std::logic_error( "Invalid Neon Palette #." );
					break;
				}
			break;

			default:
				s->format->palette->colors[ i ] = BLACK;
			break;
		}
	}
};

const sdl::color& Palette::color( int n ) const
{
	assert( n >= 0 && n < COLOR_LIMIT );
	return palettes_[ id_ ][ n ];
};

const sdl::color& Palette::bg() const
{
	if ( neon() )
	{
		return BLACK;
	}
	else if ( bg_ >= COLOR_LIMIT )
	{
		if ( Main::nextFrame( 256, 8 ) )
		{
			return color( 1 );
		}
		else
		{
			return color( 3 );
		}
	}
	else
	{
		return color( bg_ );
	}
};

Uint8 Palette::bgR() const { return bg().r; };
Uint8 Palette::bgG() const { return bg().g; };
Uint8 Palette::bgB() const { return bg().b; };
Uint8 Palette::bgA() const { return bg().a; };

void Palette::loadPalettes()
{
	const std::string file_path = Main::resourcePath() + "palettes" + Main::pathDivider() + "palettes.json";
	std::ifstream ifs( file_path );

	if ( ifs.is_open() )
	{
		rapidjson::IStreamWrapper ifs_wrap( ifs );
		rapidjson::Document pal;
		pal.ParseStream( ifs_wrap );

		assert( pal.IsObject() );
		auto palobj = pal.GetObject();
		assert( palobj.HasMember( "palettes" ) && palobj[ "palettes" ].IsArray() );

		for ( auto& pitem : palobj[ "palettes" ].GetArray() )
		{
			assert( num_o_palettes_ <= PALETTE_LIMIT );
			assert( pitem.IsObject() );
			auto p = pitem.GetObject();

			assert( p.HasMember( "name" ) && p[ "name" ].IsString() );
			const char* this_palette_name = p[ "name" ].GetString();
			
			for ( int letter = 0; letter < NAME_LIMIT; ++letter )
			{
				palette_names_[ num_o_palettes_ ][ letter ] = this_palette_name[ letter ];

				if ( this_palette_name[ letter ] == '\0' )
				{
					break;
				}
			}

			assert( p.HasMember( "colors" ) && p[ "colors" ].IsArray() );
			int color_index = 0;
			for ( auto& color_list : p[ "colors" ].GetArray() )
			{
				assert( color_list.IsObject() );
				auto color_object = color_list.GetObject();

				if ( color_object.HasMember( "r" ) && color_object[ "r" ].IsInt() ) {
					palettes_[ num_o_palettes_ ][ color_index ].r = color_object[ "r" ].GetInt();
				}
				if ( color_object.HasMember( "g" ) && color_object[ "g" ].IsInt() ) {
					palettes_[ num_o_palettes_ ][ color_index ].g = color_object[ "g" ].GetInt();
				}
				if ( color_object.HasMember( "b" ) && color_object[ "b" ].IsInt() ) {
					palettes_[ num_o_palettes_ ][ color_index ].b = color_object[ "b" ].GetInt();
				}
				if ( color_object.HasMember( "a" ) && color_object[ "a" ].IsInt() ) {
					palettes_[ num_o_palettes_ ][ color_index ].a = color_object[ "a" ].GetInt();
				}

				++color_index;
				if ( color_index >= COLOR_LIMIT ) break;
			} // COLOR_LOOP
		++num_o_palettes_;
		} // PALETTE_LOOP
	}
};

int Palette::stringID( const char* type )
{
	if ( strncmp( type, "Neon", 5 ) == 0 )
	{
		return NEON_ID;
	}

	return mezun::findIndexOfCStringInCArray( type, &palette_names_[ 0 ][ 0 ], num_o_palettes_, NAME_LIMIT );
};