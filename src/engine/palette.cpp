#include <cassert>
#include <fstream>
#include "main.hpp"
#include <iostream>
#include "mezun_exceptions.hpp"
#include "mezun_helpers.hpp"
#include "palette.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

constexpr sdl2::SDLColor Palette::BLACK;
std::unordered_map<std::string, std::array<sdl2::SDLColor, Palette::COLOR_LIMIT>> Palette::palettes_;

Palette::Palette( std::string type, int bg )
:
	type_ ( type ),
	neon_ ( ( type == "Neon" ) ? true : false ),
	bg_ ( bg )
{
	if ( palettes_.empty() )
	{
		loadPalettes();
	}
};

bool Palette::operator!= ( const Palette& p ) const
{
	return ( bg_ != p.bgN() || type_ != p.type() );
};

const Palette& Palette::operator= ( const Palette& p )
{
	type_ = p.type_;
	neon_ = p.neon_;
	bg_   = p.bg_;
};

bool Palette::neon() const { return neon_; };
const std::string& Palette::type() const { return type_; };

int Palette::bgN() const { return bg_; };

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

const sdl2::SDLColor& Palette::color( int n ) const
{
	auto p = palettes_.find( type_ );

	if ( p == palettes_.end() ) throw mezun::MissingPalette( type_ );

	try
	{
		return p->second.at( n );
	}
	catch ( const std::out_of_range& e )
	{
		throw mezun::InvalidColor( n, type_ );
	}
};

const sdl2::SDLColor& Palette::bg() const
{
	if ( neon_ )
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

int Palette::testColor( int n ) const
{
	return ( n > 0 && n < COLOR_LIMIT ) ? n : 1;
};

void Palette::loadPalettes() const
{
	const std::string file_path = Main::resourcePath() + "palettes" + Main::pathDivider() + "palettes.json";

	std::ifstream ifs( file_path );

	if ( ifs.is_open() )
	{
		rapidjson::IStreamWrapper ifs_wrap( ifs );
		rapidjson::Document pal;
		pal.ParseStream( ifs_wrap );

		if ( pal.IsObject() )
		{
			auto palobj = pal.GetObject();

			if ( palobj.HasMember( "palettes" ) && palobj[ "palettes" ].IsArray() )
			{

				for ( auto& pitem : palobj[ "palettes" ].GetArray() )
				{

					if ( pitem.IsObject() )
					{
						auto p = pitem.GetObject();

						if ( p.HasMember( "name" ) && p[ "name" ].IsString() )
						{

							std::string type = p[ "name" ].GetString();

							if ( p.HasMember( "colors" ) && p[ "colors" ].IsArray() )
							{
								sdl2::SDLColor t = { 0, 0, 0, 0 };

								std::array<sdl2::SDLColor, COLOR_LIMIT> colors = { t, t, t, t, t, t, t };

								int i = 0;
								for ( auto& c : p[ "colors" ].GetArray() )
								{
									if ( c.IsObject() )
									{
										auto cobj = c.GetObject();

										t = { 0, 0, 0, 0 };

										if ( cobj.HasMember( "r" ) && cobj[ "r" ].IsInt() )
										{
											t.r = cobj[ "r" ].GetInt();
										}
										if ( cobj.HasMember( "g" ) && cobj[ "g" ].IsInt() )
										{
											t.g = cobj[ "g" ].GetInt();
										}
										if ( cobj.HasMember( "b" ) && cobj[ "b" ].IsInt() )
										{
											t.b = cobj[ "b" ].GetInt();
										}
										if ( cobj.HasMember( "a" ) && cobj[ "a" ].IsInt() )
										{
											t.a = cobj[ "a" ].GetInt();
										}

										colors.at( i ) = t;

										++i;

										if ( i > 6 ) break;
									}
								}

								palettes_.insert
								(
									std::make_pair
									(
										type,
										colors
									)
								);
							}
						}
					}						
				}

			}

		}
	}
};