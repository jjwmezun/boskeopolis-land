#ifndef BLOCK_COMPONENT_HURT_H
#define BLOCK_COMPONENT_HURT_H

class Block;
class BlockType;
class Camera;
class Collision;
class EventSystem;
class InventoryLevel;
class GameState;
class Level;
class Sprite;

#include "block_component.h"

class BlockComponentHurt : public BlockComponent
{
	public:
		BlockComponentHurt();
		~BlockComponentHurt();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
};

#endif // BLOCK_COMPONENT_HURT_H

