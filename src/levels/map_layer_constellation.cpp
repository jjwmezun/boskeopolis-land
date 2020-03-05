#include <cassert>
#include "main.hpp"
#include "map_layer_constellation.hpp"
#include "render.hpp"
#include "unit.hpp"

namespace MapLayerConstellation
{
	static constexpr int BRIGHT_STAR_MODIFIER = 3;
	static constexpr int TILE_SIZE = Unit::PIXELS_PER_MINIBLOCK;
	static constexpr int STAR_CHANCE = 25;
	static constexpr int BIG_STAR_CHANCE = 10;
	static constexpr int MEDIUM_STAR_CHANCE = 25;
	static constexpr int BRIGHT_STAR_CHANCE = 25;
	static constexpr int MOON_START_X = Unit::WINDOW_WIDTH_MINIBLOCKS;
	static constexpr int MOON_START_Y = 3;

	static void renderTexture( int width, int height );
	static StarType drawStar();

	SDL_Texture* formTexture( const sdl2::SDLRect& src )
	{
		const int width_miniblocks = Unit::PixelsToMiniBlocks( src.w );
		const int height_miniblocks = Unit::PixelsToMiniBlocks( src.h );

		SDL_Texture* texture = Render::createRenderBox( src.w, src.h );
		Render::setRenderTarget( texture );
			Render::clearScreenTransparency();
			renderTexture( width_miniblocks, height_miniblocks );
		Render::releaseRenderTarget();
		return texture;
	};

	void renderTexture( int width, int height )
	{
		const std::string img = graphicsPath();
		sdl2::SDLRect source = { 0, 0, TILE_SIZE, TILE_SIZE };
		sdl2::SDLRect dest = { 0, 0, TILE_SIZE, TILE_SIZE };

		for ( int y = 0; y < height; ++y )
		{
			dest.x = 0;

			for ( int x = 0; x < width; ++x )
			{
				StarType star = drawStar();

				if ( star != StarType::NO_STAR )
				{
					source.x = ( ( int )( star ) % 3 ) * TILE_SIZE;
					source.y = floor( ( int )( star ) / 3 ) * TILE_SIZE;
					Render::renderObject( img, source, dest );
				}

				dest.x += TILE_SIZE;
			}

			dest.y += TILE_SIZE;
		}

		source.x = 24;
		source.y = 0;
		source.w = dest.w = 16;
		source.h = dest.h = 16;
		dest.x = MOON_START_X * TILE_SIZE;
		dest.y = MOON_START_Y * TILE_SIZE;
		Render::renderObject( img, source, dest );
	};

	StarType drawStar()
	{
		const bool draw_star = mezun::testRandomWithinPercentage( STAR_CHANCE );

		if ( draw_star )
		{
			const int star_type_percent = mezun::randPercent();
			const bool bright_star = mezun::testRandomWithinPercentage( BRIGHT_STAR_CHANCE );
			int star_type = -1;

			if ( BIG_STAR_CHANCE > star_type_percent )
			{
				star_type = ( int )( StarType::BIG_STAR );
			}
			else if ( MEDIUM_STAR_CHANCE > star_type_percent )
			{
				star_type = ( int )( StarType::MEDIUM_STAR );
			}
			else
			{
				star_type = ( int )( StarType::SMALL_STAR );
			}

			if ( bright_star )
			{
				star_type += BRIGHT_STAR_MODIFIER;
			}

			return ( StarType )( star_type );
		}
		else
		{
			return StarType::NO_STAR;
		}
	};

	std::string graphicsPath()
	{
		return "bg" + Main::pathDivider() + "constellation.png";
	};
}
