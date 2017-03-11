

// Name
//===================================
//
// Tileset
//

#ifndef TILESET_H
#define TILESET_H


// FORWARD DECLARATIONS
//===================================

	class EventSystem;


// DEPENDENCIES
//===================================

	#include "block_type.h"
    #include "rapidjson/document.h"


// CLASS
//===================================

    class Tileset
    {
        public:
			Tileset( std::string tileset = "urban" );
			
			void update( EventSystem& events );
			BlockType* blockType( int type, int x, int y );
			
		private:
			static constexpr int EMPTY_BLOCK = -1;
			static constexpr int UNIVERSAL_TILESET_SIZE = 64;
			static constexpr int TILESET_LIMIT = 400 - 64;
			
			const std::string name_;
			std::vector<std::unique_ptr<BlockType>> universal_block_types_;
			std::vector<std::unique_ptr<BlockType>> block_types_;
			
			std::vector<std::unique_ptr<BlockType>> makeBlockTypes( const std::string& tileset ) const;
			std::unique_ptr<BlockType> makeType( const rapidjson::Document& block, const std::string& tileset ) const;
    };

#endif // TILESET_H

