#pragma once

class EventSystem;

#include "animated_graphics.hpp"
#include "block_type.hpp"
#include "fading_graphics.hpp"
#include "rapidjson/document.h"
#include "sprite_graphics.hpp"

class Tileset final
{
	public:
		Tileset( std::string tileset = "urban" );

		void update( EventSystem& events );
		BlockType* blockType( int type ) const;

	private:
		static constexpr int EMPTY_BLOCK = -1;
		static constexpr int UNIVERSAL_TILESET_SIZE = 64;
		static constexpr int TILESET_LIMIT = 400 - 64;

		const std::string name_;
		std::vector<std::unique_ptr<BlockType>> universal_block_types_;
		std::vector<std::unique_ptr<BlockType>> block_types_;

		std::vector<std::unique_ptr<BlockType>> makeBlockTypes( const std::string& tileset ) const;
		std::unique_ptr<BlockType> makeType( const rapidjson::Document& block, const std::string& tileset ) const;

	template<typename T>
	std::unique_ptr<SpriteGraphics> getGraphics( const T& g, std::string texture ) const
	{
		bool flip_x = false;
		bool flip_y = false;
		double rotation = 0;
		Uint8 alpha = 255;

		if ( g.HasMember( "flip_x" ) && g[ "flip_x" ].IsBool() )
		{
			flip_x = g[ "flip_x" ].GetBool();
		}

		if ( g.HasMember( "flip_y" ) && g[ "flip_y" ].IsBool() )
		{
			flip_y = g[ "flip_y" ].GetBool();
		}

		if ( g.HasMember( "rotation" ) && g[ "rotation" ].IsInt() )
		{
			rotation = g[ "rotation" ].GetInt();
		}

		if ( g.HasMember( "alpha" ) && g[ "alpha" ].IsInt() )
		{
			alpha = g[ "alpha" ].GetInt();
		}


		if ( g.HasMember( "type" ) && g[ "type" ].IsString() )
		{
			const std::string& type_str = g[ "type" ].GetString();
			if ( "sprite" == type_str )
			{
				int x = 0;
				int y = 0;

				if ( g.HasMember( "x" ) && g[ "x" ].IsInt() )
				{
					x = g[ "x" ].GetInt();
				}

				if ( g.HasMember( "y" ) && g[ "y" ].IsInt() )
				{
					y = g[ "y" ].GetInt();
				}

				return std::make_unique<SpriteGraphics>
				(
					std::forward<std::string> ( texture ),
					Unit::BlocksToPixels( x ),
					Unit::BlocksToPixels( y ),
					flip_x,
					flip_y,
					rotation,
					0,
					0,
					0,
					0,
					alpha
				);
			}
			else if ( type_str == "animated" )
			{
				int animation_speed = 8;

				if ( g.HasMember( "speed" ) && g[ "speed" ].IsInt() )
				{
					animation_speed = g[ "speed" ].GetInt();
				}

				if ( g.HasMember( "frames" ) && g[ "frames" ].IsArray() )
				{
					std::vector<std::pair<int, int>> frames;

					for ( auto& fr : g[ "frames" ].GetArray() )
					{
						if ( fr.IsObject() )
						{
							auto f = fr.GetObject();
							int x = 0;
							int y = 0;

							if ( f.HasMember( "x" ) && f[ "x" ].IsInt() )
							{
								x = Unit::BlocksToPixels( f[ "x" ].GetInt() );
							}

							if ( f.HasMember( "y" ) && f[ "y" ].IsInt() )
							{
								y = Unit::BlocksToPixels( f[ "y" ].GetInt() );
							}

							frames.emplace_back( std::make_pair ( x, y ) );
						}
					}

					return std::make_unique<AnimatedGraphics>
					(
						std::forward<std::string> ( texture ),
						frames,
						flip_x,
						flip_y,
						rotation,
						0,
						0,
						0,
						0,
						animation_speed,
						alpha
					);
				}
			}
			if ( type_str == "fading" )
			{
				int fading_speed = 8;
				bool fading_starts_on = true;
				int x = 0;
				int y = 0;

				if ( g.HasMember( "x" ) && g[ "x" ].IsInt() )
				{
					x = g[ "x" ].GetInt();
				}

				if ( g.HasMember( "y" ) && g[ "y" ].IsInt() )
				{
					y = g[ "y" ].GetInt();
				}

				if ( g.HasMember( "speed" ) && g[ "speed" ].IsInt() )
				{
					fading_speed = g[ "speed" ].GetInt();
				}

				if ( g.HasMember( "starts_on" ) && g[ "starts_on" ].IsBool() )
				{
					fading_starts_on = g[ "starts_on" ].GetBool();
				}

				return std::make_unique<FadingGraphics>
				(
					std::forward<std::string> ( texture ),
					Unit::BlocksToPixels( x ),
					Unit::BlocksToPixels( y ),
					fading_starts_on,
					fading_speed,
					flip_x,
					flip_y,
					rotation,
					0,
					0,
					0,
					0
				);
			}
			else
			{
				return nullptr;
			}
		}
	};
};
