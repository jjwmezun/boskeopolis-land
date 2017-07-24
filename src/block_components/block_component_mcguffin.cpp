#include "block_component_mcguffin.hpp"
#include "inventory.hpp"

BlockComponentMcGuffin::BlockComponentMcGuffin() {};
BlockComponentMcGuffin::~BlockComponentMcGuffin() {};

void BlockComponentMcGuffin::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health )
{
	Inventory::addMcGuffin();
};