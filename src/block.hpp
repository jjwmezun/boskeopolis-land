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
		Block( int x, int y, BlockType* type, int location, int type_id );
		~Block();
		void render( const Camera& camera, bool priority = false ) const;
		void renderAnyPriority( const Camera& camera ) const;
		void interact( Sprite& sprite, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites );
		void destroy();
		int location() const;
		int typeID() const;
		bool hasType() const;
		const BlockType* type() const;
		bool isDestroyed() const;
		bool hasComponentType( BlockComponent::Type type ) const;
		bool testForComponentTypeNow( BlockComponent::Type type, const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const;
		void init( Map& lvmap );

	private:
		BlockType* type_; // COPY; owned by other object.
		int type_id_;
		const int location_;
};
