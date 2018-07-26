#pragma once

class Block;
class Collision;
class EventSystem;
class Health;
class Sprite;

class BlockCondition
{
	public:
		BlockCondition();
		virtual ~BlockCondition();
		virtual bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) const = 0;
};
