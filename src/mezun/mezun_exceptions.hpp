#pragma once

#include <stdexcept>
#include <SDL2/SDL.h>
#include "text.hpp"

namespace mezun
{
	class Exception : public std::runtime_error
	{
		public:
			Exception( std::u32string message );
			const std::u32string& getMessage() const;

		private:
			std::u32string message_;
	};

	class SDLWindowFailure final : public std::runtime_error
	{
		public:
			SDLWindowFailure() :
				std::runtime_error( "SDL_Window failed to load: " + std::string( SDL_GetError() ) ) {};
	};

	class SDLRendererFailure final : public std::runtime_error
	{
		public:
			SDLRendererFailure() :
				std::runtime_error( "SDL_Renderer failed to load: " + std::string( SDL_GetError() ) ) {};
	};

	class InvalidSaveSizeException final : public std::runtime_error
	{
		public:
			InvalidSaveSizeException() : std::runtime_error( "Save file is corrupt:\nfile size doesn't match expected size.\nPlease try making a new save." ) {};
	};

	inline void testSaveSize( int save_size, int size_needed )
	{
		if ( save_size < size_needed ) throw InvalidSaveSizeException();
	};

	class InvalidBlockType final : public std::runtime_error
	{
		public:
			InvalidBlockType( int id, const std::string& tileset, int x, int y ) :
				id_ (  "Invalid block type #" + Text::stringifyNum( id ) + ":" ),
				tileset_ (  "in tileset \"" + tileset + "\"" ),
				x_ (  "x: \"" + Text::stringifyNum( x ) + "\"" ),
				y_ (  "x: \"" + Text::stringifyNum( y ) + "\"" ),
				std::runtime_error
				(
					"Invalid block type #" + Text::stringifyNum( id ) + ":" +
					"\nin tileset \"" + tileset + "\"" +
					"\nx: " + Text::stringifyNum( x ) +
					"\ny: " + Text::stringifyNum( y )
				)
			{};

			void print()
			{
			};

		private:
			const std::string id_;
			const std::string tileset_;
			const std::string x_;
			const std::string y_;
	};

	class CantLoadMap final : public std::runtime_error
	{
		public:
			CantLoadMap( std::string map_tag ) :
				std::runtime_error( "Map file \"" + map_tag + "\" is missing." ) {};
	};

	class CantLoadTileset final : public std::runtime_error
	{
		public:
			CantLoadTileset( std::string tileset_tag ) :
				std::runtime_error( "Tileset file \"" + tileset_tag + "\" is missing." ) {};
	};

	class MissingTileset final : public std::runtime_error
	{
		public:
			MissingTileset( std::string tileset_tag ) :
				std::runtime_error( "Missing Tileset \"" + tileset_tag + "\"" ) {};
	};

	class MissingLevel final : public std::runtime_error
	{
		public:
			MissingLevel( const std::string& lvname );
	};

	class CorruptedLevel final : public std::runtime_error
	{
		public:
			CorruptedLevel( const std::string& lvname );
	};

	class BrokenLevelFile final : public std::runtime_error
	{
		public:
			BrokenLevelFile( const std::string& lvname );
	};

	class CantLoadLevelNames final : public std::runtime_error
	{
		public:
			CantLoadLevelNames();
	};

	class CantLoadPalettes final : public std::runtime_error
	{
		public:
			CantLoadPalettes() :
				std::runtime_error( "Can't load palettes." ) {};
	};

	class MissingPalette final : public std::runtime_error
	{
		public:
			MissingPalette( std::string type ) :
				std::runtime_error( "Missing palette \"" + type + "\"" ) {};
	};

	class InvalidColor final : public std::runtime_error
	{
		public:
			InvalidColor( int n, std::string type ) :
				std::runtime_error( "Invalid Color #" + Text::stringifyNum( n ) + " for type \"" + type + "\"" ) {};
	};

	class InvalidSprite final : public std::runtime_error
	{
		public:
			InvalidSprite( int n ) :
				std::runtime_error( "Invalid Sprite #" + Text::stringifyNum( n ) ) {};
	};
}
