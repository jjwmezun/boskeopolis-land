#include "block_component_heal.hpp"
#include "health.hpp"

BlockComponentHeal::BlockComponentHeal( int amount ) : amount_ ( amount ) {};

BlockComponentHeal::~BlockComponentHeal() {};

void BlockComponentHeal::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks )
{
	health.heal();
};