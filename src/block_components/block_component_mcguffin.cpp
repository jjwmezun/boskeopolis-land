#include "block_component_mcguffin.hpp"
#include "inventory_level.hpp"

BlockComponentMcGuffin::BlockComponentMcGuffin() {};
BlockComponentMcGuffin::~BlockComponentMcGuffin() {};

void BlockComponentMcGuffin::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	inventory.addMcGuffin();
};