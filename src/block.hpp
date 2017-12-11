#pragma once

class BlockSystem;
class BlockType;
class Health;
class Map;
class Sprite;
class SpriteGraphics;
class SpriteSystem;

#include "block_type.hpp"
#include "object.hpp"

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

		void render( const Camera& camera, bool priority = false ) const;
		void renderAnyPriority( const Camera& camera ) const;
		void interact( Sprite& sprite, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites );
		int location() const;
		int typeID() const;
		bool hasType() const;
		const BlockType* type() const;
		void destroy();
		bool hasComponentType( BlockComponent::Type type ) const;

	private:
		BlockType* type_; // COPY; owned by other object.
		int location_;
		int type_id_;
		bool destroyed_;

		bool areNearbyWithAllBlocks( const Object& other, const Map& lvmap ) const;
};