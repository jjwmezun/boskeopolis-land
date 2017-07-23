#ifndef BLOCK_COMPONENT_H
#define BLOCK_COMPONENT_H

class Block;
class BlockType;
class Camera;
class Collision;
class EventSystem;
class GameState;
class Level;
class Sprite;

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
		virtual void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera ) = 0;
		virtual void update( EventSystem& events, BlockType& type );
		Type type() const;

	protected:
		const Type type_;
};

#endif // BLOCK_COMPONENT_H