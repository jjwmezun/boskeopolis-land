#pragma once

class Block;
class BlockType;
class Collision;
class GameState;
class LevelState;
class Sprite;

class BlockComponent
{
	public:
		enum class Type
		{
			NOTYPE,
			SOLID,
			HOLE
		};

		BlockComponent( Type type = Type::NOTYPE );
		virtual ~BlockComponent();
		virtual void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const = 0;
		virtual void init( Block& block, LevelState& level_state ) const;
		Type type() const;

	protected:
		const Type type_;
};
