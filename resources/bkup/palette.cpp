#include <fstream>
#include "main.hpp"
#include <iostream>
#include "mezun_helpers.hpp"
#include "palette.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

constexpr sdl2::SDLColor Palette::BLACK;
std::vector<std::string> Palette::palette_names_;
std::vector<std::array<sdl2::SDLColor, Palette::COLOR_LIMIT>> Palette::palettes_;
std::array<sdl2::SDLColor, Palette::COLOR_LIMIT>* Palette::current_palette_ = nullptr;

Palette::Palette( std::string type, int bg )
:
	type_ ( type ),
	bg_ ( bg )
{};

bool Palette::operator!= ( const Palette& p ) const
{
	return ( bg_ != p.bgN() || !mezun::areStringsEqual( type_, p.type() ) );
};

const Palette& Palette::operator= ( const Palette& p )
{
	type_ = p.type_;
	bg_ = p.bg_;
};

const std::string& Palette::type() const { return type_; };

int Palette::bgN() const { return bg_; };

void Palette::applyPalette( SDL_Surface* s ) const
{
	assert( s != nullptr );

	for ( int i = 0; i < COLOR_LIMIT; ++i )
	{
		// Obviously we should abort if we reach beyond the # o' colors in the surface's array.
		if ( i > s->format->palette->ncolors )
		{
			return;
		}
		else
		{
			if ( isNeon() )
			{
				return;
			}
			else
			{
				s->format->palette->colors[ i ] = color( i );
			}
		}
	}
};

const sdl2::SDLColor& Palette::color( int n ) const
{
	assert( current_palette_ != nullptr );
	return current_palette_->at( n );
};

void Palette::setPalette() const
{
	if ( isNeon() )
	{
		current_palette_ = &palettes_[ 0 ];
	}
	else
	{
		const int index = idFromString( type_ );
		current_palette_ = &palettes_[ index ];
	}
};

const sdl2::SDLColor& Palette::bg() const
{
	if ( bg_ >= COLOR_LIMIT )
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
	else if ( isNeon() )
	{
		return color( 6 );
	}
	else
	{
		return color( bg_ );
	}
};

bool Palette::isNeon() const { return type_ == "Neon"; };

int Palette::testColor( int n ) const
{
	return ( n > 0 && n < COLOR_LIMIT ) ? n : 1;
};

int Palette::idFromString( const std::string& type_name )
{
	for ( int i = 0; i < palette_names_.size(); ++i )
	{
		if ( type_name == palette_names_[ i ] )
		{
			return i;
		}
	}

	mezun::error( "Invalid Palette: \"" + type_name + "\"" );
};

void Palette::loadPalettes()
{
	const std::string file_path = Main::resourcePath() + "palettes" + Main::pathDivider() + "palettes.json";
	std::ifstream ifs( file_path );
	assert( ifs.is_open() );
	rapidjson::IStreamWrapper ifs_wrap( ifs );
	rapidjson::Document pal;
	pal.ParseStream( ifs_wrap );

	assert( pal.IsObject() );
	const auto& palobj = pal.GetObject();

	assert ( palobj.HasMember( "palettes" ) && palobj[ "palettes" ].IsArray() );
	const auto& plist = palobj[ "palettes" ].GetArray();
	const int num_o_palettes = palobj[ "palettes" ].Size();
	palettes_.resize( num_o_palettes );
	palette_names_.resize( num_o_palettes );

	int pal_index = 0;
	for ( const auto& pitem : plist )
	{
		if ( !pitem.IsObject() )
		{
			continue;
		}

		const auto& p = pitem.GetObject();

		if ( !p.HasMember( "name" ) || !p[ "name" ].IsString() || !p.HasMember( "colors" ) || !p[ "colors" ].IsArray() )
		{
			continue;
		}

		palette_names_[ pal_index ] = p[ "name" ].GetString();

		int color_index = 0;
		for ( const auto& c : p[ "colors" ].GetArray() )
		{
			if ( c.IsObject() )
			{
				const auto& cobj = c.GetObject();

				assert
				(
					cobj.HasMember( "r" ) && cobj[ "r" ].IsInt() &&
					cobj.HasMember( "g" ) && cobj[ "g" ].IsInt() &&
					cobj.HasMember( "b" ) && cobj[ "b" ].IsInt() &&
					cobj.HasMember( "a" ) && cobj[ "a" ].IsInt()
				);

				palettes_[ pal_index ][ color_index ].r = cobj[ "r" ].GetInt();
				palettes_[ pal_index ][ color_index ].g = cobj[ "g" ].GetInt();
				palettes_[ pal_index ][ color_index ].b = cobj[ "b" ].GetInt();
				palettes_[ pal_index ][ color_index ].a = cobj[ "a" ].GetInt();

				++color_index;
				if ( color_index >= COLOR_LIMIT ) break;
			}
		}

		++pal_index;
	}
};