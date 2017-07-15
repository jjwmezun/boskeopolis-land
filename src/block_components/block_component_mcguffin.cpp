#include "block_component_mcguffin.h"
#include "inventory_level.h"

BlockComponentMcGuffin::BlockComponentMcGuffin() {};
BlockComponentMcGuffin::~BlockComponentMcGuffin() {};

void BlockComponentMcGuffin::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	inventory.addMcGuffin();
};