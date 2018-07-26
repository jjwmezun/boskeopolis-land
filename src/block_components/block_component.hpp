#pragma once

class Block;
class BlockSystem;
class BlockType;
class Camera;
class Collision;
class EventSystem;
class Health;
class GameState;
class Level;
class Sprite;
class SpriteSystem;

class BlockComponent
{
	public:
		enum class Type
		{
			NOTYPE,
			SOLID
		};

		BlockComponent( Type type = Type::NOTYPE );
		virtual ~BlockComponent();
		virtual void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const = 0;
		Type type() const;

	protected:
		const Type type_;
};
