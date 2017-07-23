#include "block_component_money.hpp"
#include "inventory.hpp"

BlockComponentMoney::BlockComponentMoney( int amount ) : amount_ ( amount ) {};

BlockComponentMoney::~BlockComponentMoney() {};

void BlockComponentMoney::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera )
{
	Inventory::addFunds( amount_ );
};
