#ifndef BLOCK_CONDITION_H
#define BLOCK_CONDITION_H

class Block;
class Collision;
class EventSystem;
class Sprite;

class BlockCondition
{
	public:
		BlockCondition();
		virtual ~BlockCondition();
		virtual bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) = 0;
};

#endif // BLOCK_CONDITION_H

