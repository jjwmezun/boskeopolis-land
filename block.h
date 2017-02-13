

// Name
//===================================
//
// Block
//

#ifndef BLOCK_H
#define BLOCK_H


// FORWARD DECLARATIONS
//===================================

    class BlockType;
    class Map;
    class Sprite;
    class SpriteGraphics;


// DEPENDENCIES
//===================================

    #include "block_type.h"
    #include "object.h"


// CLASS
//===================================

    class Block : public Object
    {
        public:
            friend class BlockSystem;

            Block( int x, int y, BlockType* type, int location, int type_id );
            ~Block();
            Block( Block&& m );
            Block( const Block& c );
            Block& operator=( Block&& m );
            Block& operator=( const Block& c );

            void render( Graphics& graphics, Camera& camera, bool priority = false );
            void interact( Sprite& sprite, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
            int location() const;
            int typeID() const;
            void destroy();

        private:
            BlockType* type_ = nullptr; // COPY; owned by other object.
            int location_;
            int type_id_;
            bool destroyed_;
    };

#endif // BLOCK_H
