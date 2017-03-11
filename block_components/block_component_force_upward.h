#ifndef BLOCK_COMPONENT_FORCE_UPWARD_H
#define BLOCK_COMPONENT_FORCE_UPWARD_H

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

class BlockComponentForceUpward : public BlockComponent
{
	public:
		BlockComponentForceUpward();
		~BlockComponentForceUpward();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera );
};

#endif // BLOCK_COMPONENT_FORCE_UPWARD_H

