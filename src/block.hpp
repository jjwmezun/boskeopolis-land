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

class Block final : public Object
{
	public:
		Block( int x, int y, BlockType* type, int location, int type_id );
		~Block();
		void render( const Camera& camera, bool priority = false ) const;
		void renderAnyPriority( const Camera& camera ) const;
		void interact( Sprite& sprite, LevelState& level );
		void destroy();
		int location() const;
		int typeID() const;
		bool hasType() const;
		const BlockType* type() const;
		bool isDestroyed() const;
		bool hasComponentType( BlockComponent::Type type ) const;
		bool hasComponentTypeWhetherCollided( BlockComponent::Type type ) const;
		bool testForComponentTypeNow( BlockComponent::Type type, const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const;
		void init( LevelState& level_state );

	private:
		BlockType* type_; // COPY; owned by other object.
		int type_id_;
		const int location_;
};
