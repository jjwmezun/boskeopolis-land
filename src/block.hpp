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
		void render( const Camera& camera, bool priority = false, SDL_Texture* texture = nullptr ) const;
		void renderAnyPriority( const Camera& camera ) const;
		void interact( Sprite& sprite, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites );
		void destroy();
		int location() const;
		int typeID() const;
		bool hasType() const;
		const BlockType* type() const;
		bool isDestroyed() const;
		bool hasComponentType( BlockComponent::Type type ) const;
		void init( Map& lvmap );

	private:
		BlockType* type_; // COPY; owned by other object.
		int type_id_;
		const int location_;
};
