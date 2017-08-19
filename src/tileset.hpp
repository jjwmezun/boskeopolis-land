#pragma once

class EventSystem;

#include "block_type.hpp"
#include "rapidjson/document.h"

class Tileset
{
	public:
		static constexpr int EMPTY_BLOCK = -1;
		static constexpr int UNIVERSAL_TILESET_SIZE = 64;
		static constexpr int TILESET_LIMIT = 400 - 64;

		Tileset( int id = 0 );

		void update( EventSystem& events );
		BlockType* blockType( int type, int x, int y );
	
		static void loadTilesetNames();
		static int nameID( const char* target_name );

	private:
		static constexpr int NAME_LIMIT = 50;
		static constexpr int LETTER_LIMIT = 10;

		static char tileset_names_[ NAME_LIMIT ][ LETTER_LIMIT ];
		static int num_o_tilesets_;

		std::vector<std::unique_ptr<BlockType>> block_types_;

		std::vector<std::unique_ptr<BlockType>> makeBlockTypes( int tileset_id ) const;
		std::unique_ptr<BlockType> makeType( const rapidjson::Document& block, int tileset_id ) const;
};