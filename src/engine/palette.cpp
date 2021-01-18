#include <fstream>
#include "game_state.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "mezun_exceptions.hpp"
#include "mezun_helpers.hpp"
#include "palette.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "wmessage_state.hpp"

static std::unordered_map<std::string, std::array<sdl2::SDLColor, Palette::COLOR_LIMIT>> palettes_;
static std::vector<std::string> palette_names_;

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
	return *this;
};

const std::string& Palette::type() const { return type_; };

int Palette::bgN() const
{
	return bg_;
};

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
				s->format->palette->colors[ i ] = color( i );
			}
		}
	}
};

sdl2::SDLColor Palette::color( unsigned int n ) const
{
	auto p = palettes_.find( type_ );
	if ( p == palettes_.end() )
	{
		Main::pushState( std::unique_ptr<WMessageState> ( WMessageState::generateErrorMessage( Localization::getCurrentLanguage().getMissingPaletteErrorMessage( type_ ), WMessageState::Type::POP, nullptr ) ) );
		return { 0, 0, 0, 0 };
	}
	return p->second.at( n );
};

sdl2::SDLColor Palette::bg() const
{
	return color( bgN() );
};

Uint8 Palette::bgR() const { return bg().r; };
Uint8 Palette::bgG() const { return bg().g; };
Uint8 Palette::bgB() const { return bg().b; };
Uint8 Palette::bgA() const { return bg().a; };

unsigned int Palette::testColor( unsigned int n ) const
{
	return ( n > 0 && n < COLOR_LIMIT ) ? n : 1;
};

void Palette::init()
{
	// Create default palette as backup.
	palettes_.insert
	(
		std::make_pair
		(
			"Grayscale",
			std::array<sdl2::SDLColor, Palette::COLOR_LIMIT> {
				sdl2::SDLColor{ 255, 255, 255, 0   },
				sdl2::SDLColor{ 255, 255, 255, 255 },
				sdl2::SDLColor{ 200, 200, 200, 255 },
				sdl2::SDLColor{ 150, 150, 150, 255 },
				sdl2::SDLColor{ 100, 100, 100, 255 },
				sdl2::SDLColor{  50,  50,  50, 255 },
				sdl2::SDLColor{   0,   0,   0, 255 }
			}
		)
	);
	palette_names_.push_back( "Grayscale" );

	const std::string file_path = Main::resourcePath() + "palettes" + Main::pathDivider() + "palettes.json";
	std::ifstream ifs( file_path );
	if ( !ifs.is_open() )
	{
		throw mezun::Exception( U"Missing palettes file.\nPlease redownload game." );
	}

	rapidjson::IStreamWrapper ifs_wrap( ifs );
	rapidjson::Document pal;
	pal.ParseStream( ifs_wrap );
	if ( !pal.IsObject() )
	{
		throw mezun::Exception( U"Palettes file has become corrupted.\nPlease redownload game." );
	}
	const auto& palobj = pal.GetObject();
	if ( !palobj.HasMember( "palettes" ) || !palobj[ "palettes" ].IsArray() )
	{
		throw mezun::Exception( U"Palettes file has become corrupted.\nPlease redownload game." );
	}

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

					palette_names_.push_back( type );
				}
			}
		}						
	}
};

PaletteChanger::PaletteChanger() : number_ ( 0 ) {};
void PaletteChanger::setCurrentPalette( GameState* state, int bg )
{
	state->newPalette( { palette_names_[ number_ ], bg } );
	printf( "Current Palette: %s\n", palette_names_[ number_ ].c_str() );
};

void PaletteChanger::setNextPalette( GameState* state, int bg )
{
	++number_;
	if ( number_ >= palette_names_.size() )
	{
		number_ = 0;
	}
	setCurrentPalette( state, bg );
};

void PaletteChanger::setPreviousPalette( GameState* state, int bg )
{
	--number_;
	if ( number_ < 0 )
	{
		number_ = palette_names_.size() - 1;
	}
	setCurrentPalette( state, bg );
};