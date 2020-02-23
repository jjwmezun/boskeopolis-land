#pragma once

class Block;
class Collision;
class LevelState;
class Sprite;

class BlockCondition
{
	public:
		BlockCondition();
		virtual ~BlockCondition();
		virtual bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const = 0;
};
