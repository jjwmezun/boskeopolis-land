#pragma once

class Block;
class BlockCondition;
class Collision;
class EventSystem;
class Health;
class GameState;
class Level;
class Sprite;
class SpriteGraphics;

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
		void interact( Collision& collision, Sprite& sprite, Block& block, Level& level, EventSystem& events, Camera& camera, Health& health );
		void render( const Camera& camera, const Block& block, bool priority ) const;
		void renderAnyPriority( const Camera& camera, const Block& block ) const;

		bool hasComponentType( BlockComponent::Type type ) const;
		void rotate( Direction::Clockwise direction );
		void readjust();
		const SpriteGraphics* graphics() const;


	private:
		static constexpr int BLOCK_SIZE = 16;
		static constexpr int MINI_BLOCK_SIZE = 8;
		static constexpr int NUM_O_MINI_BLOCKS = 4;

		std::unique_ptr<SpriteGraphics> graphics_;
		std::vector<std::unique_ptr<BlockComponent>> components_;
		std::vector<std::vector<std::unique_ptr<BlockCondition>>> conditions_;

		Direction::Clockwise prev_direction_;
};