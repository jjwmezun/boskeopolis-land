#pragma once

class Block;
class BlockCondition;
class BlockSystem;
class Collision;
class EventSystem;
class Health;
class GameState;
class Level;
class Sprite;
class SpriteGraphics;
class SpriteSystem;

#include "block_component.hpp"
#include <map>
#include "object.hpp"
#include <vector>

class BlockType
{
	public:
		friend class BlockComponentSwitch;

		BlockType
		(
			std::unique_ptr<SpriteGraphics> = nullptr,
			std::vector<std::unique_ptr<BlockComponent>> components = {},
			std::vector<std::vector<std::unique_ptr<BlockCondition>>> conditions = {}
		);

		~BlockType();
		BlockType( const BlockType& ) = delete;
		BlockType( BlockType&& ) = default;
		const BlockType& operator= ( const BlockType& ) = delete;
		const BlockType&& operator= ( BlockType&& ) = delete;

		void update( EventSystem& events );
		void interact( Collision& collision, Sprite& sprite, Block& block, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites );
		void render( const sdl2::SDLRect& dest, bool priority, const Camera* camera = nullptr ) const;
		void renderAnyPriority( const sdl2::SDLRect& dest, const Camera* camera = nullptr ) const;
		void init( Block& block, Map& lvmap ) const;

		bool hasComponentType( BlockComponent::Type type ) const;
		bool testForComponentTypeNow( BlockComponent::Type type, const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const;
		const SpriteGraphics* graphics() const;


	private:
		std::unique_ptr<SpriteGraphics> graphics_;
		std::vector<std::unique_ptr<BlockComponent>> components_;
		std::vector<std::vector<std::unique_ptr<BlockCondition>>> conditions_;

		bool testCanInteract( int i, const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const;
};
